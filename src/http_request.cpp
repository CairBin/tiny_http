#include "tiny_http/http_request.h"
#include <sstream>


namespace tiny_http {

const std::string HTTP_VERSION = "HTTP/1.1";

const std::string HttpResponse::GetStatusMessage(int status_code) {
    switch (status_code) {
        case 200:
            return "200 OK";
        case 201:
            return "201 Created";
        case 202:
            return "202 Accepted";
        case 203:
            return "203 Non - Authoritative Information";
        case 204:
            return "204 No Content";
        case 205:
            return "205 Reset Content";
        case 206:
            return "206 Partial Content";
        case 300:
            return "300 Multiple Choices";
        case 301:
            return "301 Moved Permanently";
        case 302:
            return "302 Found";
        case 303:
            return "303 See Other";
        case 304:
            return "304 Not Modified";
        case 305:
            return "305 Use Proxy";
        case 307:
            return "307 Temporary Redirect";
        case 400:
            return "400 Bad Request";
        case 401:
            return "401 Unauthorized";
        case 402:
            return "402 Payment Required";
        case 403:
            return "403 Forbidden";
        case 404:
            return "404 Not Found";
        case 405:
            return "405 Method Not Allowed";
        case 406:
            return "406 Not Acceptable";
        case 407:
            return "407 Proxy Authentication Required";
        case 408:
            return "408 Request Time - out";
        case 409:
            return "409 Conflict";
        case 410:
            return "410 Gone";
        case 411:
            return "411 Length Required";
        case 412:
            return "412 Precondition Failed";
        case 413:
            return "413 Request Entity Too Large";
        case 414:
            return "414 Request - URI Too Long";
        case 415:
            return "415 Unsupported Media Type";
        case 416:
            return "416 Requested range not satisfiable";
        case 417:
            return "417 Expectation Failed";
        case 500:
            return "500 Internal Server Error";
        case 501:
            return "501 Not Implemented";
        case 502:
            return "502 Bad Gateway";
        case 503:
            return "503 Service Unavailable";
        case 504:
            return "504 Gateway Time - out";
        case 505:
            return "505 HTTP Version not supported";
        default:
            return "Unknown Status Code";
    }
}

std::string HttpResponse::ToString() const {
    std::ostringstream oss;
    oss << HTTP_VERSION
        << " "
        << GetStatusMessage(status_code_)
        << "\r\n";

    for(const auto& header : headers_){
        oss << header.first
            << ":"
            << header.second
            << "\r\n";
    }
    oss << "\r\n"
        << body_;

    return oss.str();
}

HttpResponse& HttpResponse::SetStatus(unsigned int code){
    status_code_ = code;
    return *this;
}

HttpResponse& HttpResponse::Send(const std::string& content){
    body_ = content;
    return *this;
}

HttpResponse& HttpResponse::Set(const std::string& key, const std::string& value){
    headers_[key] = value;
    return *this;
}

HttpResponse& HttpResponse::Type(const std::string& type){
    headers_["Content-type"] = type;
    return *this;
}



}