#pragma once 

#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>



class Client {
    private :
        int sockfd;

        
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

        std::vector<char> read_buffer;
        std::vector<char> write_buffer;


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
        int read_data(int sockfd) {
            ssize_t bytes_recvd;
            char buffer[1024];

            while (true) {
                bytes_recvd = recv(sockfd, buffer, sizeof(buffer), 0);
                if (bytes_recvd < 0) {
                    std::cerr << "Error reading data" << std::endl;
                }
                read_buffer.insert(read_buffer.end(), buffer, buffer + bytes_recvd);
                if (bytes_recvd < sizeof(buffer)) {
                    break; // No more data to read
                }
            }
            return bytes_recvd;
        }
    
};