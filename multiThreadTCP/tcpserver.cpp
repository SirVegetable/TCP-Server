#include "tcpserver.hpp"
#include <pthread.h>
#include <strings.h>


TCPserver::TCPserver()
{
    isListening = false; 
    listeningPort = -1;
    pthread_mutex_init(&mutex,nullptr);
}

TCPserver::~TCPserver()
{
    pthread_mutex_destroy(&mutex); 
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

void TCPserver::Listen(int port, std::function<void(TCPserver* srv,Connection* src,const std::string&)> callBack){

    if(isListening)
        return;
    this->listeningPort = port;
    this->callBack = callBack; 
    
    // start thread
    if(pthread_create(&listenerThread,nullptr,StartListeningProc,this)!= 0){
        throw "listening thread: failed to create"; 
    }
}

void TCPserver::ListenThreadProc(){
    
    socketId = socket(AF_INET,SOCK_STREAM,0);
    if(socketId < 0){
        std::cout << socketId << std::endl; 
        throw "Error creating socket"; 
    }
    int enable = 1;
    if(setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        throw "Error: SO_REUSEADDR"; 
    }
    bzero((char*)&serverAddr,sizeof(serverAddr));
    serverAddr.sin_family = AF_UNSPEC;
    serverAddr.sin_port = htons(listeningPort);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if(bind(socketId, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0 ){
        throw "Error: Binding Socket";
    }
    listen(socketId,5);
    isListening = true; 
    while(isListening){
        Connection* clientConn = new Connection(this);
        if(!clientConn->Accept(socketId)){
            delete clientConn; 
            break; 
        }
        std::cout << "connection established..." << std::endl; 
        pthread_mutex_lock(&mutex); 
        this->clientQueue.push_back(clientConn);
        pthread_mutex_unlock(&mutex); 
    }
    isListening = false; 
}   
void TCPserver::MessageRecieved(Connection* src, const std::string& msg){
    callBack(this,src,msg); 
}

void TCPserver::SendResponse(Connection* dst, const std::string& msg){
    dst->Send(msg); 
}
void TCPserver::Stop(){
    

}



