#include "client.h"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <unordered_map>
#include <fcntl.h>
#include <cstring>

#pragma once

class Server {
    private:
        #define MAX_EVENTS 64
        struct epoll_event ev, events[MAX_EVENTS];
        int listen_sockfd, epollfd, conn_sockfd, nfds;

        std::unordered_map<int, Client> clients;
        char buffer[1024];

        void set_nonblocking(auto snfd) {
            int flags = fcntl(snfd, F_GETFL, 0);
            fcntl(snfd, F_SETFL, flags | O_NONBLOCK);
        }
    

    public: 
        Server() {
            std::cout << "Starting Server\n";
            struct sockaddr_in server_addr;
            memset(&server_addr, 0 , sizeof(server_addr));

            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = INADDR_ANY;
            std::cout<<inet_ntoa(server_addr.sin_addr) << std::endl;
            server_addr.sin_port = htons(8080);
            
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            memset(&client_addr, 0, addr_len);
                        
            int listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (listen_sockfd < 0)
            {
                perror("socket");
                exit(EXIT_FAILURE);
            }

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
                        for(;;) {
                            
                            std::cout<<";;;;" << std::endl;
                            std::cout << events[n].data.fd << ".,.,.,.," << listen_sockfd << std::endl;
                            std::cout<<";;;;" << std::endl;

                            int client_fd = accept(listen_sockfd, (struct sockaddr *) &client_addr, &addr_len);
                            if (client_fd <0){
                                if (errno == EAGAIN || errno == EWOULDBLOCK){
                                    break;
                                }
                                break;
                            }else {
                                std::cout << "Connection accepted ip: " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;
                                
                                set_nonblocking(client_fd);
                                
                                clients.insert({client_fd, Client()});
                                std::cout <<client_fd << " added to clients map" << std::endl;
                                std::cout << "::::" << std::endl;
                                std::cout << (client_fd == listen_sockfd) << std::endl;
                                std::cout << "::::" << std::endl;
                                
                                struct epoll_event c_ev;
                                c_ev.events = EPOLLIN | EPOLLET;
                                c_ev.data.fd = client_fd;

                                epoll_ctl(epollfd, EPOLL_CTL_ADD, client_fd, &c_ev);
                            }
                        }
                    } else {
                        auto curr_fd = events[n].data.fd;
                         
                    }
                }
            }
        }        
    };