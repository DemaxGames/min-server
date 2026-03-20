#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "Socket.h"
#include "json.h"

int main(int argc, const char *argv[]){
    printf("Hello min server!\n");
    
    json::Field test;
    std::ifstream file;
    std::string str;
    file.open("example.json", std::ios::ate);
    if(file){
        auto size = file.tellg();
        str.resize((size_t)size);
        file.seekg(0);
        file.read(str.data(), str.size());
    }
    test.FillFromStr(str);

    if(test.name == "car"){
        std::cout << "name: car\n";
        std::cout << "\tengine : " << *(std::string*)(test["engine"].GetField()) << "\n";
        std::cout << "\thorse_power : " << *(int*)(test["horse_power"].GetField()) << "\n";
        std::cout << "\tseatbelt : " << *(bool*)(test["seatbelt"].GetField()) << "\n";
        std::cout << "\tlicense : " << (test["license"].GetField()) << "\n";
    }


    // Socket s(8080);
    // std::string answer;
    
    // // Load index.html into answer with HTTP headers
    // std::ifstream file("index.html", std::ios::ate);
    // if (file){
    //     auto size = file.tellg();
    //     answer.resize(static_cast<size_t>(size));
    //     file.seekg(0);
    //     file.read(answer.data(), answer.size());
    // }
    
    // while(true){
    //     std::string message = s.Receive();
    //     std::cout << message << "\n";
    //     s.Send(answer);
    // }

    return 0;
}