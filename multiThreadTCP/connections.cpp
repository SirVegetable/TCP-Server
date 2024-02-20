#include "connections.hpp"
#include <arpa/inet.h>
#include <pthread.h>

Connection::Connection()
{

}

Connection::~Connection()
{

}

void* Connection_monitorThreadProc(void* param){
    Connection* clientC = static_cast<Connection*>(param);
    try{
        clientC->MonitorThreadProc(); 
    }
    catch(...){
        std::cerr << "Error in MonitorThreadProc"<< std::endl; 
    }
    delete clientC;

    return nullptr; 
}

bool Connection::Accept(int listeningSocket){
    clientAddrSize = sizeof(clientAddr); 
    connSocket = accept(listeningSocket,&clientAddr, &clientAddrSize);
    if(connSocket < 0){
        return false; 
    }
    if(pthread_create(&monitorThread,nullptr, Connection_monitorThreadProc,this) < 0){
        throw "Error: creating monitor thread"; 
    }
    if(pthread_detach(monitorThread) != 0){
        throw "Error: detaching monitor thread"; 
    }
    return true; 
}

void Connection::MonitorThreadProc(){

}

void Connection::Send(const std::string msg){
    
}
