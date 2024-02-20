#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP
#include <iostream>
#include <vector> 
#include "connections.hpp"

class TCPserver{
    public: 
        TCPserver(); 
        ~TCPserver(); 

    private: 
        std::vector<Connection*> clientQueue; 

}; 



#endif