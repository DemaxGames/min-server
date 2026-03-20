#include "json.h"

#include <iostream>
#include <vector>

namespace json{

Field::Field(){
    type = typeid(void).hash_code();
    data = NULL;
}

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
    } else if(type == typeid(std::vector<Field>).hash_code()){
        delete (std::vector<Field>*)data;
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
    } else if(type == typeid(std::vector<Field>).hash_code()){
        return (std::vector<Field>*)data;
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
            } else if(type == typeid(std::vector<Field>).hash_code()){
                delete (std::vector<Field>*)data;
            }
        }
        data = var;
    } else {
        std::cerr << "ERROR: cannot set field with name " << name << " because of the uknown type\n";
    }
}

Field& Field::operator[] (std::string name){
    if(type == typeid(std::vector<Field>).hash_code()){
        for(int i = 0; i < ((std::vector<Field>*)data)->size(); i++){
            if((*((std::vector<Field>*)data))[i].name == name) return (*((std::vector<Field>*)data))[i];
        }
    }
}

const Field& Field::operator[] (std::string) const{
    if(type == typeid(std::vector<Field>).hash_code()){
        for(int i = 0; i < ((std::vector<Field>*)data)->size(); i++){
            if((*((std::vector<Field>*)data))[i].name == name) return (*((std::vector<Field>*)data))[i];
        }
    }
}

int Field::FillFromStr(std::string str){
    bool array = false;
    int i = 0;
    int name_start;
    for(; str[i] != '"'; i++);
    name_start = ++i;
    for(; str[i] != '"'; i++);
    int name_len = i - name_start;
    name = str.substr(name_start, name_len);
    std::cout << "name: " << name << "\n";

    int data_start;
    int data_len;
    for(; str[i] != ':'; i++);
    data_start = ++i;
    for(; str[i] == ' '; i++);
    if(str[i] == '{'){
        array = true;
        i++;
        int opened_brackets = 1;
        for(; opened_brackets != 0; i++){
            if(str[i] == '{') opened_brackets++;
            if(str[i] == '}') opened_brackets--;
        }
        i++;
    }
    for(; str[i] != ',' && str[i] != '}'; i++);
    data_len = i - data_start;
    std::string data = str.substr(data_start, data_len);
    std::cout << "data: " << data << "\n";
    if(array){
        int j = 0;
        while(data.size() > 1){
            //std::cout << "j: " << j << " feedstr: " << data << " feedstrsize: " << data.size() << "\n";
            j = FillFromStr(data);
            data = data.substr(j + 1, data.size() - j);
        }
    }
    return i;
}


}