#include <iostream>
#include "tiny_http/http_server.h"

int main(){
    tiny_http::HttpServer server(8080);
    if(server.Initialize() < 0){
        std::cerr << "Initialize failed" << std::endl;
        return -1;
    }
    if(server.Bind() < 0){
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }
    if(server.Listen() < 0 ){
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }
    std::cout << "Server started on port "
        << server.get_port() 
        << "..."
        << std::endl;
    
    return 0;
}