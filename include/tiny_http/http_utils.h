#ifndef HTTP_UTILS_H_
#define HTTP_UTILS_H_

namespace tiny_http{

int ReadLine(int socket_fd, char* buffer, unsigned long buffer_size, bool keep_newline = false);

}

#endif // HTTP_UTILS_H_