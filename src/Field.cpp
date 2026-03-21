#include "json.h"

#include <iostream>
#include <vector>
#include <ctype.h>

bool spaceCheck(std::string str){
    int i = 0;
    int s = 0;
    for(; i < str.size(); i++){
        if(isspace(str[i])) s++;
        if(str[i] == '\0') s++;
    }
    return i != s;
}

namespace json{

Field::Field(){
    type = typeid(void).hash_code();
    data = NULL;
}

Field::Field(Field &&obj){
    // std::cout << "Move semantics\n";
    name = obj.name;
    type = obj.type;
    data = obj.data;
    obj.data = NULL;
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
        std::cerr << "ERROR: cannot find object with name: " << name << "\n"; 
    }
    std::cerr << "ERROR: wrong field type to []: " << name << "\n"; 
    return *this;
}

const Field& Field::operator[] (std::string name) const{
    if(type == typeid(std::vector<Field>).hash_code()){
        for(int i = 0; i < ((std::vector<Field>*)data)->size(); i++){
            if((*((std::vector<Field>*)data))[i].name == name) return (*((std::vector<Field>*)data))[i];
        }
        std::cerr << "ERROR: cannot find object with name: " << name << "\n"; 
    }
    std::cerr << "ERROR: wrong field type to []: " << name << "\n"; 
    return *this;
}

int Field::FillFromStr(std::string str){
    bool array = false;
    int i = 0;
    int name_start;
    for(; str[i] != '"' && i < str.size(); i++);
    if(i == (str.size() - 1)) return i;
    name_start = ++i;
    for(; str[i] != '"'; i++);
    int name_len = i - name_start;
    name = str.substr(name_start, name_len);
    // std::cout << "name: " << name << "\n";

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
    }
    for(; str[i] != ',' && str[i] != '}'; i++);
    data_len = i - data_start;
    std::string data = str.substr(data_start, data_len); 
    // std::cout << "data: " << data << "\n";
    if(array){
        type = typeid(std::vector<Field>).hash_code();
        this->data = new std::vector<Field>;
        int j = 0;
        while(data.size() > 1){
            if(!spaceCheck(data)) break;
            Field tmp;
            j = tmp.FillFromStr(data);
            data = data.substr(j + 1, data.size() - j);
            (*(std::vector<Field>*)(this->data)).push_back(std::move(tmp));
        }
    } else {
        bool number = true;
        this->data = NULL;
        for(int j = 0; j < data.size(); j++){
            if(data[j] == '"') {
                //std::cout << "trying to convert a string\n";
                type = typeid(std::string).hash_code();
                int str_start = ++j;
                for(;data[j] != '"'; j++);
                int str_len = j - str_start;
                this->data = new std::string(data.substr(str_start, str_len));
                number = false;
                break;
            } else if(data[j] == 'n'){
                if(data.substr(j, 4) == "null"){
                    type = typeid(void).hash_code();
                    this->data = NULL;
                }
                number = false;
                break;
            } else if(data[j] == 't' || data[j] == 'f'){
                if(data.substr(j, 4) == "true"){
                    type = typeid(bool).hash_code();
                    this->data = new bool(true);
                } else if(data.substr(j, 5) == "false"){
                    type = typeid(bool).hash_code();
                    this->data = new bool(false);
                }
                number = false;
                break;
            }
        }
        if(number){
            bool dot = false;
            for(int j = 0; j < data.size(); j++) if(data[j] == '.') dot = true;
            if(dot){
                type = typeid(double).hash_code();
                this->data = new double(std::stod(data));
            } else {
                // std::cout << "Trying to stoi: " << data << "\n";
                type = typeid(int).hash_code();
                this->data = new int(std::stoi(data));
            }
        }
    }

    return i;
}

std::string Field::Stringify(){
    std::string str = "\"" + name + "\":";
    if(type == typeid(int).hash_code()){
        str += std::to_string(*(int*)data);
    } else if(type == typeid(double).hash_code()){
        str += std::to_string(*(double*)data);
    } else if(type == typeid(bool).hash_code()){
        str += std::to_string(*(bool*)data);
    } else if(type == typeid(std::string).hash_code()){
        str += "\"" +(*(std::string*)data) + "\"";
    } else if(type == typeid(std::vector<Field>).hash_code()){
        str += "{\n";
        for(int i = 0; i < (*(std::vector<Field>*)data).size(); i++){
            str += (*(std::vector<Field>*)data)[i].Stringify();
            if(i != ((*(std::vector<Field>*)data).size() - 1)) str += ",\n";
        }
        str += "\n}";
    } else if(type == typeid(void).hash_code()){
        str += "null";
    }
    return str;
}

json::Field &json::Field::operator=(json::Field const &obj){
    name = obj.name;
    type = obj.type;
    data = obj.data;
    return *this;
}


}