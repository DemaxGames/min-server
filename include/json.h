#pragma once
#include <typeinfo>
#include <string>
#include <vector>

namespace json{

class Field{
public:
    std::string name;
    size_t type;
    void* data;

    Field();
    Field(Field &&obj);
    Field(std::string name, size_t hashcode);
    ~Field();

    Field& operator[] (std::string);
    const Field& operator[] (std::string) const;
    Field &operator=(Field const &obj);

    void* GetField();
    void SetField(size_t hashcode, void* var);
    int FillFromStr(std::string str);
    std::string Stringify();
};

class File{
public:
    Field top;

    File();
    File(std::string path);

    void Read(std::string str);
    std::string Write();
};

}