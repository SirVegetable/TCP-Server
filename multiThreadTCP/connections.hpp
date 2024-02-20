#ifndef CONNECTIONS_HPP
#define CONNECTIONS_HPP

#include <iostream>
#include <string> 
#include <sys/socket.h>

class Connection{
    public: 
        Connection(); 
        ~Connection(); 

        bool Accept(int listeningSocket);
        void Send(const std::string msg); 
    private: 
        int connSocket;
        sockaddr clientAddr;
        socklen_t clientAddrSize; 
        
};

#endif