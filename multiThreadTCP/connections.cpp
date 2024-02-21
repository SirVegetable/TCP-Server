#include "connections.hpp"
#include "tcpserver.hpp"
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>



Connection::Connection(TCPserver* server)
{   
    this->server = server; 
    isConnected = false;
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
        std::cerr << "Error in MonitorThreadProc "<< std::endl; 
    }
    delete clientC;

    return nullptr; 
}

bool Connection::Accept(int listeningSocket){
    clientAddrSize = sizeof(clientAddr); 
    connSocket = accept(listeningSocket,&clientAddr, &clientAddrSize);
    std::cout << "hdurr\n"; 
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
    const int bufferSize = 2048;
    char buffer[bufferSize + 1];
    int n;
    isConnected = true;
    while(true){
        n = read(connSocket, &buffer, bufferSize);
        if( n <= 0){
            break;
        }
        std::string msg(buffer, n);
        server->MessageRecieved(this, msg); 
        
    }
    close(connSocket); 
    isConnected = false; 
}

void Connection::Send(const std::string msg){
    if(!isConnected){
        throw "Sending on unconnected socket"; 
    }
    size_t check = write(connSocket,&msg[0], msg.size());
    if(check < msg.size()){
        throw "Error sending message";
    }
}

