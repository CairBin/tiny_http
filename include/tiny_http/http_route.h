#ifndef _HTTP_ROUTE_H_
#define _HTTP_ROUTE_H_

#include <string>
#include <functional>
#include "tiny_http/http_request.h"

namespace tiny_http{
// 定义带参数的请求处理函数类型
using ParameterizedRequestHandler = std::function<void(const HttpRequest&, HttpResponse&, const std::unordered_map<std::string, std::string>&)>;
class Route{
private:
    typedef struct RouteEntry{
        std::string regex_pattern_;
        std::vector<std::string> param_names;
        ParameterizedRequestHandler handler_;
    }RouteEntry;

private:
    std::unordered_map<std::string, std::vector<RouteEntry> > route_map_;

public:
    void AddRoute(const std::string& method, const std::string& route_pattern, const ParameterizedRequestHandler& handler);
    void Get(const std::string& route_pattern, const ParameterizedRequestHandler& handler);
    void Post(const std::string& route_pattern, const ParameterizedRequestHandler& handler);
    void HandleRequest(const HttpRequest& request, HttpResponse& response) const;

private:
    void ParseRoutePattern(const std::string& pattern, std::string& regex_pattern, std::vector<std::string>&param_names);

};

}


#endif