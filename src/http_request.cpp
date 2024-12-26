#include "tiny_http/http_request.h"
#include <sstream>


namespace tiny_http {

const std::string HTTP_VERSION = "HTTP/1.1";

const std::string HttpResponse::GetStatusMessage(int status_code){
    switch(status_code){
        case 200:
            return "200 OK";
        case 404:
            return "404 Not Found";
        default:
            return "Unknown Status Code";
    }
}


std::string HttpResponse::ToString() const {
    std::ostringstream oss;
    oss << HTTP_VERSION
        << status_code_
        << " "
        << GetStatusMessage(status_code_)
        << "\r\n";

    for(const auto& header : headers_){
        oss << header.first
            << ": "
            << header.second
            << "\r\n";
    }
    oss << "\r\n"
        << body_;

    return oss.str();
}

}