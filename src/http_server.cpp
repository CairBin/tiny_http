#include "tiny_http/http_server.h"
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include "tiny_http/http_parser.h"
#include <cstring>
#include "tiny_http/http_utils.h"
#include <iostream>

namespace tiny_http {

const uint64_t MAX_BUFFER = 1024;


Port HttpServer::get_port() const {
    return this->port_;
}

int HttpServer::Initialize() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)!= 0) {
        return -1;
    }
#endif

    // 创建套接字
    socket_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ == -1) {
        #ifdef _WIN32
        return -1;
        #else
        return -1;
        #endif
    }
    // 设置端口可复用
    int opt = 1;
    int ret = -1;
    #ifdef _WIN32
    ret = setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
    #else
    ret = setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    #endif

    if(ret == -1){
        return -1;
    }
}

int HttpServer::Bind() {
    // 配置服务器网络地址
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = PF_INET;  // 网络套接字
    server_addr.sin_port = htons(port_);  // 端口
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 绑定套接字
    int ret = bind(socket_, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(ret < 0){
        return -1;
    }

    int name_len = sizeof(server_addr);
    if(port_ == 0){
        #ifdef _WIN32
        ret = getsockname(socket_, (struct sockaddr*)&server_addr, &name_len);
        #else
        ret = getsockname(socket_, (struct sockaddr*)&server_addr, (socklen_t*)&name_len);
        #endif
        if(ret < 0)
            return -1;
        port_ = server_addr.sin_port;
    }
}

void HandleConnection(int client_sock, std::unique_ptr<IRouter>& router){
    if(client_sock < 0) return;

    char buffer[MAX_BUFFER];    // 缓冲区
    HttpRequestParser parser;   // 请求解析器

    bool error_flag = false;
    printf("-----------------starting-----------------\n");
    while(1){
        if(parser.IsDone()) break;
        int char_nums = ReadLine(client_sock, buffer, MAX_BUFFER, false);   // 不获取换行符
        if(char_nums < 0){
            error_flag = true;
            break;
        }

        try{
            parser.Parse(std::string(buffer, buffer + char_nums));
        }catch(const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            error_flag = true;
            break;
        }
    }
    printf("----------------------------------------------------------------\n");

    if(error_flag){
        close(client_sock);
        return;
    }
    HttpRequest request;
    request.method_ = parser.method();
    request.body_ = parser.body();
    request.path_ = parser.url();
    request.version_ = parser.version();
    request.headers_ = parser.headers();

    HttpResponse response;
    if(!router->HandleRequest(parser.url(), request, response)){
        std::cerr << "Error: Error route handler." << std::endl;
        close(client_sock);
        return;
    }

    close(client_sock);
}

int HttpServer::Listen(){
    if(listen(socket_, listen_queue_size_) < 0){
        return -1;
    }
    
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    while(1){
        int client_sock = -1;
        #ifdef _WIN32
        client_sock = accept(
            socket_, 
            (struct sockaddr*)&client_addr, 
            &client_addr_len
        );
        #else
        client_sock = accept(
            socket_, 
            (struct sockaddr*)&client_addr, 
            (socklen_t*)&client_addr_len
        );
        #endif

        if(client_sock == -1){
            return -1;
        }
        pool_->Submit(HandleConnection, client_sock, std::ref(router_));
    }
}




}