#pragma once
#include <winsock2.h>
#include <string>

class Socket{
public:
    SOCKET sock;
    SOCKET client;
    u_short port;
    
    Socket(u_short p);
    ~Socket();

    std::string Receive();
    void Send(std::string message);
    

};