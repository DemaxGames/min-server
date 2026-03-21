#include "Socket.h"
#include <ws2tcpip.h>
#include <iostream>
#include <ctime>
#include <iomanip>

Socket::Socket(u_short p){
    port = p;

    WSAData wsadata;
    int wsaresult;
    wsaresult = WSAStartup(MAKEWORD(2, 2), &wsadata);

    if(wsaresult != 0){
        std::cerr << "ERROR: canot start WSA\n" 
        << "\tFailed with error code: " << wsaresult << "\n";
        return;
    }
    
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == INVALID_SOCKET){
        std::cerr << "ERROR: cannot create socket\n";
        return;
    }

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int bindresult = bind(sock, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr_in));
    if(bindresult != 0){
        std::cerr << "ERROR: cannot bind socket\n";
        return;
    }
}

Socket::~Socket(){
    closesocket(sock);
}

std::string Socket::Receive(){
    
    int listenresult = listen(sock, SOMAXCONN);
    if(listenresult != 0){
        std::cerr << "ERROR: listen failed with error " << WSAGetLastError() << "\n";
    }
    
    sockaddr_in clientAddr;
    int clientAddrLength;
    client = accept(sock, (sockaddr*)&clientAddr, &clientAddrLength);
    
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    char ip_string[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), ip_string, INET_ADDRSTRLEN);
    std::cout << std::put_time(&tm, "[%H:%M:%S] ") << "Accepted packet from:\n"
    << "\tip: " << ip_string << "\n"
    << "\tport: " << ntohs(clientAddr.sin_port) << "\n";
    
    char buffer[64 * 1024];
    for(int i = 0; i < 64 * 1024; i++) buffer[i] = 0;
    recv(client, buffer, 64 * 1024, 0);
    
    std::string message(buffer);
    return message;
}

void Socket::Send(std::string message){
    send(client, message.data(), message.size(), 0);
}