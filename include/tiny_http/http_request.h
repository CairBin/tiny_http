#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include <cstring>
#include <string>
#include <unordered_map>
namespace tiny_http{
class HttpRequest{
public:
    std::string method_;
    std::string path_;
    std::string version_;
    std::string body_;
};

class HttpResponse{
public:
    int status_code_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;

public:
    std::string ToString() const;

private:
    static const std::string GetStatusMessage(int status_code);
};

}

#endif