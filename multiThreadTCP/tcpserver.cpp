#include "tcpserver.hpp"
#include<pthread.h>


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

void TCPserver::Listen(int port, std::function<void(const std::string&)> callBack){

    if(isListening) return;
    this->listeningPort = port;
    this->callBack = callBack; 
    
    // start thread
    if(pthread_create(&listenerThread,nullptr,StartListeningProc,this)!= 0){
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

void TCPserver::ListenThreadProc(){
    int socketId = socket(AF_UNSPEC,SOCK_STREAM,0);
    if(socketId < 0){
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
        Connection* clientConn = new Connection();
        if(!clientConn->Accept(socketId)){
            delete clientConn; 
            break; 
        }
        pthread_mutex_lock(&mutex); 
        this->clientQueue.push_back(clientConn);
        pthread_mutex_unlock(&mutex); 
    }
}   

void TCPserver::Stop(){


}



