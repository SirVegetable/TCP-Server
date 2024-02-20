#ifndef CONNECTIONS_HPP
#define CONNECTIONS_HPP

#include "tcpserver.hpp"
#include <iostream>
#include <string> 
#include <sys/socket.h>
#include <pthread.h>

class Connection{
    public:
        Connection()=default; 
        Connection(TCPserver *server); 
        ~Connection(); 

        bool Accept(int listeningSocket);
        void Send(const std::string msg);
        void MonitorThreadProc(); 
    private:
        TCPserver* server; 
        int connSocket;
        bool isConnected; 
        sockaddr clientAddr;
        socklen_t clientAddrSize;
        pthread_t monitorThread; 
        
};

#endif