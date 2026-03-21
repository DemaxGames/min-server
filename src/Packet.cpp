#include "packet.h"

#include <iostream>

#define VERSION "0.1"

namespace packet{

std::string GetHeader(std::string type, std::string version){
    return type + "\\" + version; 
}

std::string OK::WritePacket(){
    std::string str = GetHeader("OK", VERSION);
    return str;
}

std::string MESSAGE::WritePacket(){
    std::string str = GetHeader("MESSAGE", VERSION);
    str += "\n<json>\n";

    json::File json;
    json.top = json::Field("json", typeid(std::vector<json::Field>).hash_code());
    
    json.top.data = new std::vector<json::Field>;
    std::vector<json::Field> *fields = (std::vector<json::Field> *)json.top.data;
    fields->push_back(json::Field("message", typeid(std::string).hash_code()));
    fields->push_back(json::Field("time", typeid(std::string).hash_code()));
    fields->push_back(json::Field("owner", typeid(std::string).hash_code()));

    json.top["message"].data = new std::string(message);
    json.top["time"].data = new std::string(time);
    json.top["owner"].data = new std::string(owner);

    str += json.Write();

    return str;
}


void SEND::ReadPacket(std::string str){
    int i = 0;
    while(i < str.size()){
        for(; str[i] != '<'; i++);
        if(str.substr(i, 6) == "<json>"){
            str = str.substr(i + 7, str.size() - (i + 7));
            // std::cout << "got substr: " << str;
            break;
        }
    }
    if(i == (str.size() - 1)) return;
    std::cout << "JSON TEST1: " << str << "\n";

    json::File json;
    json.Read(str);
    std::cout << "JSON TEST2: " << json.Write() << "\n";

    std::string *json_message = (std::string*)json.top["message"].GetField();
    std::string *json_owner = (std::string*)json.top["owner"].GetField();

    if(json_message != NULL) message = *json_message;
    else std::cerr << "ERROR: cannot get message from SEND packet\n";
    if(json_owner != NULL) owner = *json_owner;
    else std::cerr << "ERROR: cannot get owner from SEND packet\n";
}

void GET::ReadPacket(std::string str){
int i = 0;
    while(i < str.size()){
        for(; str[i] != '<'; i++);
        if(str.substr(i, 6) == "<json>"){
            str = str.substr(i + 7, str.size() - (i + 7));
            // std::cout << "got substr: " << str << "\n";
            break;
        }
    }
    if(i == (str.size() - 1)) return;
    std::cout << "JSON TEST1: " << str << "\n";

    json::File json;
    json.Read(str);


    std::cout << "JSON TEST2: " << json.Write() << "\n";
    int *json_id = (int*)json.top["id"].GetField();

    if(json_id != NULL) id = *json_id;
    else std::cerr << "ERROR: cannot get id from GET packet\n";
}

}