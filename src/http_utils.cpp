#include "tiny_http/http_utils.h"
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
// #pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif


namespace tiny_http{

int ReadLine(int socket_fd, char* buffer, unsigned long buffer_size, bool keep_newline){
  if (!buffer || buffer_size == 0) {
    return -1;
  }

  size_t bytes_read = 0;
  bool last_was_cr = false;
  char c;

  while (bytes_read < buffer_size - 1) {  // 保留最后一个字节给 \0
    ssize_t result = recv(socket_fd, &c, 1, 0);
    if (result < 0) {
      return -1;
    } else if (result == 0) {
      // 连接关闭
      break;
    }

    if (c == '\n') {
      if (keep_newline) {
        if (last_was_cr) {
          buffer[bytes_read++] = '\r';
        }
        buffer[bytes_read++] = '\n';
      }
      break;
    }

    if (last_was_cr) {
      // 如果之前的字符是 \r，而当前不是 \n
      if (keep_newline) {
        buffer[bytes_read++] = '\r';
      }
      last_was_cr = false;
    }

    if (c == '\r') {
      last_was_cr = true;
    } else {
      buffer[bytes_read++] = c;
    }
  }

  buffer[bytes_read] = '\0';  // 确保以 null 结尾
  return bytes_read;
}

}