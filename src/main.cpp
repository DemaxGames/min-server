#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>


#include "Socket.h"
#include "json.h"
#include "packet.h"

packet::Type DetectType(std::string msg){
    int i = 0;
    for(; msg[i] != '\\'; i++);
    std::string str = msg.substr(0, i);
    if(str == "OK") return packet::Type::OK;
    if(str == "MESSAGE") return packet::Type::MESSAGE;
    if(str == "SEND") return packet::Type::SEND;
    if(str == "GET") return packet::Type::GET;
    return packet::Type::NONE;
}

std::string GetTime(){
    std::time_t timestamp;
    std::time(&timestamp);
    std::string datetime = std::ctime(&timestamp);
    return datetime;
}

int main(int argc, const char *argv[]){
    printf("Hello min server!\n");

    std::vector<packet::MESSAGE> chat;

    Socket s(8080);
    std::string answer;
        
    while(true){
        std::string message = s.Receive();
        std::cout << message << "\nend\n";
        std::string answer;

        //logic

        packet::Type t = DetectType(message);
        
        switch(t){
            case packet::Type::SEND:
            {
                packet::SEND send;
                send.ReadPacket(message);

                packet::MESSAGE msg;
                msg.message = send.message;
                msg.time = GetTime();
                msg.owner = send.owner;
                chat.push_back(msg);

                packet::OK packet;
                answer = packet.WritePacket();
                break;
            }
            case packet::Type::GET:
            {
                packet::GET get;
                get.ReadPacket(message);

                std::cout << "get.id: " << get.id << "\n";

                if(get.id >= 0 && get.id < chat.size()){
                    packet::MESSAGE msg = chat[get.id];
                    answer = msg.WritePacket();
                } else{
                    packet::OK ok;
                    answer = ok.WritePacket();
                }
            }
        }
        

        std::cout << "answer: \n" << answer << "\n";
        s.Send(answer);
        message.clear();
    }

    return 0;
}