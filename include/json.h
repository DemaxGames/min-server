#pragma once
#include <typeinfo>
#include <string>

namespace json{

class Field{
public:
    std::string name;
    size_t type;
    void* data;
    Field(std::string name, size_t hashcode);
    ~Field();
    void* GetField();
    void SetField(size_t hashcode, void* var);
};

}