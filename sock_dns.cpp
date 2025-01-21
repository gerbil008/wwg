#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string_ops.h>
#include <nlohmann/json.hpp>

#define dns_port 6968
#define allowed_cons 10
#define buffer_size 512

using json =  nlohmann::json;


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

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[buffer_size] = {0};
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(dns_port);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    for(;;){
        listen(server_fd, allowed_cons);
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        read(new_socket, buffer, buffer_size);
        log(buffer);
        const char* content = read_json_entry(buffer).c_str();
        send(new_socket, content, strlen(content), 0);
        close(new_socket);
    }
    return 0;
}
