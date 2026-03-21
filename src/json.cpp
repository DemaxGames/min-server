#include "json.h"
#include <iostream>

namespace json{

File::File(){
    
}

void File::Read(std::string str){
    std::string trick = "{\"json\" : " + str + "\n}\n";
    top.FillFromStr(trick);

}

std::string File::Write(){
    std::string str = top.Stringify();
    str.erase(0, std::string("json : ").size());
    return str;
}


}