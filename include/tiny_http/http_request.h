#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

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
    std::unordered_map<std::string, std::string> headers_;
    std::unordered_map<std::string, std::string> params_;
};

class HttpResponse{
public:
    int status_code_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;

public:
    std::string ToString() const;
    HttpResponse& SetStatus(unsigned int code);
    HttpResponse& Send(const std::string& content);
    HttpResponse& Append(const std::string& ctx);
    HttpResponse& Set(const std::string& key, const std::string& value);
    HttpResponse& Type(const std::string& type);

private:
    static const std::string GetStatusMessage(int status_code);
};

}

#endif // HTTP_REQUEST_H_