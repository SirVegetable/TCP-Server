#include "tcpserver.hpp"
#include<pthread.h>


TCPserver::TCPserver()
{
    isListening = false; 
    listeningPort = -1; 
}

TCPserver::~TCPserver()
{

}

void TCPserver::Listen(int port, std::function<void(const std::string&)> callBack){

    if(isListening) return;
    this->listeningPort = port;
    this->callBack = callBack; 
    
    // start thread
    if(pthread_create(&listenerThread,nullptr,StartListeningProc(),this)!= 0){
        throw "listening thread: failed to create"; 
    }
}
void* StartListeningProc(void* param){
    TCPserver* server = static_cast<TCPserver*>(param); 
    try{
        server->ListenThreadProc(); 
    }
    catch(...){
        std::cerr << "Listening thread error" << std::endl; 
    }
    return nullptr; 
}

void TCPserver::Stop(){


}



