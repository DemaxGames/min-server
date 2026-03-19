#include <stdio.h>
#include <string>
#include "Socket.h"
#include <iostream>
#include <fstream>

int main(int argc, const char *argv[]){
    printf("Hello min server!\n");
    
    Socket s(8080);
    std::string answer;
    
    // Load index.html into answer with HTTP headers
    std::ifstream file("index.html", std::ios::ate);
    if (file) {
        auto size = file.tellg();
        answer.resize(static_cast<size_t>(size));
        file.seekg(0);
        file.read(answer.data(), answer.size());
    }
    
    while(true){
        std::string message = s.Receive();
        std::cout << message << "\n";
        s.Send(answer);
    }

    return 0;
}