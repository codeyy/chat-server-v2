#pragma once 

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cstdint>
#include <arpa/inet.h>



class Client {
    private :
    int sockfd;    
    
    public :
        std::string read_buffer; 

        Client(int sock): sockfd(sock){
            if (sockfd < 0) {
                std::cerr << "Error creating socket" << std::endl;
            }
        }
        Client(const Client &source) = delete;
        Client& operator=(const Client &source) = delete;

        Client(Client&& source) noexcept {
            sockfd = source.sockfd;
            read_buffer = std::move(source.read_buffer);
            source.sockfd = -1;
        }
        Client& operator=(Client&& source) noexcept {
            if (this != &source){
                if (sockfd >=0) close(sockfd);
                sockfd = source.sockfd;
                read_buffer = std::move(source.read_buffer);
                source.sockfd = -1;
            }
            return *this;
        }
        
        ~Client() {
            if (sockfd >= 0) {
                close(sockfd);
            }
        }

        auto get_socket() const -> int {
            return sockfd;
        }

        short read_data(int sockfd) {
            read_buffer.clear();
            
            
            uint32_t target_bytes = 4;
            
            while (target_bytes > 0){
                char buffer[4];
                int res = recv(sockfd, buffer, target_bytes, 0);
                if (res <= 0){
                    return -1;
                }
                read_buffer.insert(read_buffer.end(), buffer, buffer + res);
                target_bytes -= res;
                
            }
            
            std::memcpy(&target_bytes, read_buffer.data(), sizeof(target_bytes));
            target_bytes = ntohl(target_bytes);
            read_buffer.clear();
            std::vector<char> buffer(target_bytes);
            while (0 < target_bytes){
                int res = recv(sockfd, buffer.data(), target_bytes, 0);
                if (res <= 0){
                    break;
                }
                read_buffer.insert(read_buffer.end(), buffer.begin(), buffer.begin() + res);
                target_bytes-=res;
                buffer.resize(target_bytes);
            }
            read_buffer.insert(read_buffer.end(), '\0' );

            return 0;
        }
    
};