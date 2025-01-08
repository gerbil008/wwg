#include <nlohmann/json.hpp>
#include <string_ops.h>
#include <iostream>
#include <string>

using json = nlohmann::json;

void log(std::string msg){
    std::cout<<msg<<std::endl;
}


std::string read_json_entry(std::string entry){
    std::string fileContent = read_file("dns.json");
    json jarray = json::parse(fileContent);
    for (auto& [key, value] : jarray.items()) {
        if(key == entry){
            json endarray = json::parse(value.dump(4));
            return endarray[0];
        }
    }
    return "";
}

int main(void){
    log(read_json_entry("example2.com"));
    return 0;
}