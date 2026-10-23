#include "client.h"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include <unordered_map>
#include <fcntl.h>
#include <netinet/in.h>

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
            sockaddr_in server_addr;
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(8080);
            server_addr.sin_addr.s_addr = INADDR_ANY;
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            
            std::cout<< 1;
            int listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (listen_sockfd < 0)
            bind(listen_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
            listen(listen_sockfd, SOMAXCONN);

            while(true){
                nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
                if (nfds == -1) {
                    perror("epoll_wait");
                    break;
                }
            
                for (int n =0; n <nfds; n++){
                    int curr_fd = events[n].data.fd;
                    for(;;) {
                        int client_fd = accept(listen_sockfd, (struct sockaddr *) &client_addr, &addr_len);
                        if (client_fd <0){
                            if (errno == EAGAIN || errno == EWOULDBLOCK){
                                break;
                            }
                            break;
                        }
                        set_nonblocking(client_fd);
                    
                        clients[client_fd] = Client();
                        std::cout <<client_fd;
                        epoll_ctl(epollfd, EPOLL_CTL_ADD, client_fd, &ev); 
                    }
                    if (curr_fd == listen_sockfd) {

                        for(;;) {
                            int client_fd = accept(listen_sockfd, (struct sockaddr *) &client_addr, &addr_len);
                            if (client_fd == -1) {
                                if (errno == EAGAIN || errno == EWOULDBLOCK) break; // No more clients
                                break; // Error occurred
                            }
                            set_nonblocking(client_fd);
                    
                            clients[client_fd] = Client();
                            epoll_ctl(epollfd, EPOLL_CTL_ADD, client_fd, &ev); 
                        }
                    } else {
                        std::cout<<"handle data";
                    }
                }
            }
                }
            
            
                
        };         