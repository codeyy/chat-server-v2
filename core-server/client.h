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
    
    std::vector<char> read_buffer;
    //std::vector<char> write_buffer;
    
    std::string username;
    std::string ip_address;
    int state; // 0 - disconnected, 1 - connected, 2 - authenticated
        
        
    public :
        Client(): sockfd(-1){
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                std::cerr << "Error creating socket" << std::endl;
            }
        }
        ~Client() {
            if (sockfd >= 0) {
                close(sockfd);
            }
        }

        auto get_socket() const -> int {
            return sockfd;
        }   
        std::string get_username() const {
            return username;
        }
        std::string get_ip() const {
            return ip_address;
        }


        void set_username(std::string un){ username = un;}
        void set_ip(std::string ip){ ip_address = ip;}

        auto read_data(int sockfd) {
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