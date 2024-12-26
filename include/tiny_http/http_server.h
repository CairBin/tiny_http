#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include <cstdint>
#include <string>
#include "tiny_http/thread_pool.hpp"

namespace tiny_http{

using Port = uint16_t;
const Port DEFAULT_PORT = 80;
const int LISTEN_QUEUE_SIZE = 10;
const uint64_t THREAD_POOL_SIZE = 10;

class HttpServer{
private:
    Port port_;
    int socket_;
    int listen_queue_size_;
    ThreadPool* pool_;

public:
    HttpServer() : HttpServer(DEFAULT_PORT, LISTEN_QUEUE_SIZE, THREAD_POOL_SIZE) {}
    HttpServer(Port port) : HttpServer(port, listen_queue_size_, THREAD_POOL_SIZE) {}
    HttpServer(Port port, int listen_queue_size, uint64_t pool_size) : port_(port), listen_queue_size_(listen_queue_size) {
        pool_ = new ThreadPool(pool_size);
    }
    ~HttpServer() {
        delete pool_;
    }
    int Initialize();
    int Bind();
    int Listen();

public:
    Port get_port() const;
};

}


#endif