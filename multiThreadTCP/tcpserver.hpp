#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP
#include "connections.hpp"
#include <iostream>
#include <vector>
#include <functional> 
#include <pthread.h>
#include <arpa/inet.h>


class TCPserver{
    public: 
        TCPserver(); 
        ~TCPserver();

        void Listen(int port,std::function<void(const std::string&)> callBack);
        void ListenThreadProc();
        void Stop();
        void MessageRecieved(Connection* src, const std::string& msg);

    private:
        int listeningPort;
        int socketId;
        struct sockaddr_in serverAddr; 
        volatile bool isListening;
        pthread_t listenerThread;
        pthread_mutex_t mutex; 
        std::function<void(const std::string&)> callBack; 
        std::vector<Connection*> clientQueue; 

}; 



#endif