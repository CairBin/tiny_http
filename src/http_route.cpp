#include "tiny_http/http_route.h"
#include <regex>

namespace tiny_http {

void Route::AddRoute(const std::string&method, const std::string&route_pattern, const ParameterizedRequestHandler& handler){
    std::string regex_pattern;
    std::vector<std::string> param_names;
    ParseRoutePattern(route_pattern, regex_pattern, param_names);
    route_map_[method].emplace_back(regex_pattern, param_names, handler);
}

void Route::Get(const std::string& route_pattern, const ParameterizedRequestHandler& handler){
    AddRoute("GET", route_pattern, handler);
}

void Route::Post(const std::string& route_pattern, const ParameterizedRequestHandler& handler){
    AddRoute("POST", route_pattern, handler);
}

void Route::HandleRequest(const HttpRequest& request, HttpResponse& response) const{
    auto it = route_map_.find(request.method_);
        if (it!= route_map_.end()) {
            for (const auto& entry : it->second) {
            std::smatch match;
            if (std::regex_search(request.path_, match, std::regex(entry.regex_pattern_))) {
                std::unordered_map<std::string, std::string> params;
                for (size_t i = 1; i < match.size(); ++i) {
                    params[entry.param_names[i - 1]] = match[i].str();
                }
                entry.handler_(request, response, params);
                return;
            }
        }
    }
    response.status_code_ = 404;
    response.headers_["Content-Type"] = "text/plain";
    response.body_ = "Not Found";
}

void Route::ParseRoutePattern(const std::string& pattern, std::string& regex_pattern, std::vector<std::string>& param_names){
    regex_pattern = "";
    param_names.clear();
    size_t pos = 0;
    while (pos < pattern.size()) {
        if (pattern[pos] == '{') {
            size_t end_pos = pattern.find('}', pos);
            if (end_pos!= std::string::npos) {
                param_names.push_back(pattern.substr(pos + 1, end_pos - pos - 1));
                regex_pattern += "([^/]+)";
                pos = end_pos + 1;
            } else {
                // 处理模式错误
                throw std::invalid_argument("Invalid route pattern: " + pattern);
            }
        } else {
            if (pattern[pos] == '.') {
                regex_pattern += "\\.";
            } else {
                regex_pattern += pattern[pos];
            }
            pos++;
        }
    }
}

}