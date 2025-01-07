#ifndef _HTTP_UTILS_H_
#define _HTTP_UTILS_H_

namespace tiny_http{

int ReadLine(int socket_fd, char* buffer, unsigned long buffer_size, bool keep_newline = false);

}

#endif