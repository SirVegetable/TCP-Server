#include "connections.hpp"
#include <arpa/inet.h>

Connection::Connection()
{

}

Connection::~Connection()
{
    
}

bool Connection::Accept(int listeningSocket){
    connSocket = accept(listeningSocket,&clientAddr, &clientAddrSize); 
}
void Connection::Send(const std::string msg){

}
