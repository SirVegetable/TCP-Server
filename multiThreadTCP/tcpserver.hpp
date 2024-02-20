#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP
#include "connections.hpp"
#include <iostream>
#include <vector>
#include <functional> 
#include <pthread.h>

class TCPserver{
    public: 
        TCPserver(); 
        ~TCPserver();

        void Listen(int port,std::function<void(const std::string&)> callBack);
        void ListenThreadProc(int port, std::function<void(const std::string&)> callback);  
        void Stop();  

    private:
        int listeningPort;
        bool isListening;
        pthread_t listenerThread; 
        std::function<void(const std::string&)> callBack; 
        std::vector<Connection*> clientQueue; 

}; 



#endif