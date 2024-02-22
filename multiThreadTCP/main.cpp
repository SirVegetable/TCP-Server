#include <iostream> 
#include "connections.hpp"
#include "tcpserver.hpp"

int main(){

    TCPserver server;
    server.Listen(12345,[](TCPserver* srv,Connection* src,const std::string& msg){
        src->Send(msg); 
        std::cout << msg << std::endl;
        
    });

    int foo;
    std::cin >>foo;  
    return 0; 
}
