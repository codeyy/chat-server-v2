#include "client.hpp"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <unordered_map>
#include <fcntl.h>
#include <cstring>
#include <memory>


#pragma once

class Server {
    private:
        #define MAX_EVENTS 64
        struct epoll_event ev, events[MAX_EVENTS];
        int listen_sockfd, epollfd, conn_sockfd, nfds;

        std::unordered_map<int, Client> clients;
        std::unordered_map<std::string, int> usernames;
        char buffer[1024];

        void set_nonblocking(auto snfd) {
            int flags = fcntl(snfd, F_GETFL, 0);
            fcntl(snfd, F_SETFL, flags | O_NONBLOCK);
        }
        std::vector<std::string> parser(std::string msg){ //we'll make it run on a separate thread later
            //msg protocol : recepient>message>sender>
            std::vector<std::string> parsed_msg(3);
            
            int store_index=0;
            for (int i = 0; i < msg.size(); i++){
                if(msg[i] != '>'){parsed_msg[store_index] += msg[i];
                }else{store_index++;}
                if(store_index >= 3){break;}
            }
            return parsed_msg;
        }
    

    public: 
        Server() {
            std::cout << "Starting Server\n";
            struct sockaddr_in server_addr;
            memset(&server_addr, 0 , sizeof(server_addr));


            int listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (listen_sockfd < 0)
            {
                perror("socket");
                exit(EXIT_FAILURE);
            }


            int opt = 1;
            if (setsockopt(listen_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
                std::cerr << "setsockopt(SO_REUSEADDR) failed\n";
                close(listen_sockfd);
                exit(EXIT_FAILURE);
            }
            

            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = INADDR_ANY;
            server_addr.sin_port = htons(8080);
            std::cout<<inet_ntoa(server_addr.sin_addr) <<  std::endl;
            
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            memset(&client_addr, 0, addr_len);
                        

            bind(listen_sockfd, (const struct sockaddr*)&server_addr, sizeof(server_addr));
            listen(listen_sockfd, SOMAXCONN);
            set_nonblocking(listen_sockfd);
            epollfd = epoll_create1(0);
            ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = listen_sockfd;
            epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sockfd, &ev);

            while(true){
                while(true) {
                    try {
                        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
                        break;
                    } catch (const std::exception& e) {
                        if (errno == EINTR) {
                            std::cout << "Interrupted by signal, retrying..." << std::endl;
                            continue; // Interrupted by signal, retry
                        }
                        std::cerr << "epoll_wait error: " << e.what() << std::endl;
                    }
                }
            
                for (int n =0; n <nfds; n++){
                    if (events[n].data.fd == listen_sockfd) {
                        for(;;){
                            int client_fd = accept(listen_sockfd, (struct sockaddr *) &client_addr, &addr_len);
                            if (client_fd <0){
                                if (errno == EAGAIN || errno == EWOULDBLOCK){
                                    break;
                                }
                                break;
                            }else {                                
                                set_nonblocking(client_fd);
                                
                                clients.insert({client_fd, Client()});
                                
                                struct epoll_event c_ev;
                                c_ev.events = EPOLLIN | EPOLLET;
                                c_ev.data.fd = client_fd;

                                epoll_ctl(epollfd, EPOLL_CTL_ADD, client_fd, &c_ev);
                            }
                        }
                    } else {
                        int curr_fd = events[n].data.fd;
                        if (clients.contains(curr_fd)) {
                            auto ClientO = clients[curr_fd];
                            ClientO.read_data(curr_fd);
                            if (ClientO.read_buffer.empty()) {
                                epoll_ctl(epollfd, EPOLL_CTL_DEL, curr_fd, nullptr);
                                clients.erase(curr_fd);
                                close(curr_fd);
                            } else {
                                std::vector<std::string> received = parser(ClientO.read_buffer);
                                
                                if ((received[0]) == "Server"){
                                    if(!usernames.contains(received[1])){usernames[received[1]] = curr_fd;}
                                }else{
                                    if(usernames.contains(received[0])){
                                        auto recp_fd = usernames[received[0]];
                                        std::string final_msg = received[0] + ">" + received[1] + ">" + received[2] + ">";
                                        ssize_t bytes_sent = send(recp_fd, final_msg.c_str(), final_msg.length(), 0);
                                    }else{
                                        
                                    }
                                }
                            }
                        } else {
                            std::cerr << "Client not found for fd: " << curr_fd << std::endl;
                        } 
                    }
                }
            }
        }
        
        ~Server(){
            for (const auto& [key, value] : usernames){
                close(value);
            }
            clients.clear();
            usernames.clear();
            std::cout<< "Server shutdown completed successfully";
        }
    };