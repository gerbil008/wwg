#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string_ops.h>
#include <string>

#define port 6969
#define buffer_size 4098

const char* path = "./websites/";

void log(std::string msg){
    std::cout<<msg<<std::endl;
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
    address.sin_port = htons(port);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    for(;;){
        listen(server_fd, 3);
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        read(new_socket, buffer, buffer_size);
        log(buffer);
        std::string buf(buffer);
        std::string filename = path;
        filename += buf;
        std::cout<<"filename is: "<<filename<<std::endl;
        std::string content = read_file(filename.c_str());
        std::cout<<"server sent: "<<content.c_str()<<std::endl;
        send(new_socket, content.c_str(), strlen(content.c_str()), 0);
        close(new_socket);
    }
    return 0;
}
