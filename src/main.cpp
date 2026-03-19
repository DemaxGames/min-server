#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "Socket.h"
#include "json.h"

int main(int argc, const char *argv[]){
    printf("Hello min server!\n");
    
    json::Field test("test", typeid(int).hash_code());
    test.SetField(typeid(int).hash_code(), new int(56));
    std::cout << "int: " <<*(int*)test.GetField() << "\n";
    test.SetField(typeid(int).hash_code(), new int(12));
    std::cout << "int: " <<*(int*)test.GetField() << "\n";
    test.SetField(typeid(int).hash_code(), new int(-99));
    std::cout << "int: " <<*(int*)test.GetField() << "\n";
    test.SetField(typeid(float).hash_code(), new float(21.8f));
    std::cout << "float: " << *(float*)test.GetField() << "\n";
    std::cout << "tests done\n";


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