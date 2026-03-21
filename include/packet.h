#pragma once

#include "json.h"

namespace packet{

enum class Type{
    OK,
    MESSAGE,
    SEND,
    GET,
    NONE
};

class Generic{
public:
    Type type;
    void* data;
};

class OK{
public:
    std::string WritePacket();
};

class MESSAGE{
public:
    std::string message;
    std::string time;
    std::string owner;
    std::string WritePacket();
};

class SEND{
public:
    std::string message;
    std::string owner;
    void ReadPacket(std::string str);
};

class GET{
public:
    int id;
    void ReadPacket(std::string str);
};

}