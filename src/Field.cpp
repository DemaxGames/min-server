#include "json.h"

#include <iostream>

namespace json{

Field::Field(std::string name, size_t hashcode){
    this->name = name;
    type = hashcode;
    data = NULL;
}

Field::~Field(){
    if(data == NULL) return;
    if(type == typeid(int).hash_code()){
        delete (int*)data;
    } else if(type == typeid(double).hash_code()){
        delete (double*)data;
    } else if(type == typeid(bool).hash_code()){
        delete (bool*)data;
    } else if(type == typeid(std::string).hash_code()){
        delete (std::string*)data;
    } else {
        std::cerr << "ERROR: cannot properly delete field with name " << name <<", memory leaks expected\n";
    }
}

void* Field::GetField(){
    if(data == NULL) return NULL;
    if(type == typeid(int).hash_code()){
        return (int*)data;
    } else if(type == typeid(double).hash_code()){
        return (double*)data;
    } else if(type == typeid(bool).hash_code()){
        return (bool*)data;
    } else if(type == typeid(std::string).hash_code()){
        return (std::string*)data;
    } else {
        std::cerr << "ERROR: cannot return field with name " << name << " because of the uknown type\n";
        return NULL;
    }
}

void Field::SetField(size_t hashcode, void* var){
    if(type == hashcode){
        if(data != NULL){
            if(type == typeid(int).hash_code()){
                delete (int*)data;
            } else if(type == typeid(double).hash_code()){
                delete (double*)data;
            } else if(type == typeid(bool).hash_code()){
                delete (bool*)data;
            } else if(type == typeid(std::string).hash_code()){
                delete (std::string*)data;
            }
        }
        data = var;
    } else {
        std::cerr << "ERROR: cannot set field with name " << name << " because of the uknown type\n";
    }
}

}