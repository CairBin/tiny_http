#ifndef HTTP_ROUTER_H_
#define HTTP_ROUTER_H_

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include "tiny_http/http_request.h"

namespace tiny_http{

enum class RouterNodeType{
    STATIC,     // 非根节点普通字符串节点
    ROOT,       // 根节点
    PARAM,      // 参数节点
    CATCH_ALL   // 通配符节点
};

using HttpHandler = std::function<void(HttpRequest&, HttpResponse&)>;

class RouterTireTree{
private:
    typedef struct Node{
        std::string path_;
        bool wild_child_;
        RouterNodeType n_type_;
        std::string indices_;

        std::unordered_map<char, Node*> children_;
        std::vector<std::string> param_names_;
        HttpHandler handler_;;
    } Node;

private:
    Node* root_;

private:
    void InsertNode(Node* parent, const std::string& path, int start, int end, HttpHandler handler);
    Node* FindNode(Node *parent, const std::string &path, int start, int end, std::unordered_map<std::string, std::string>& param_values);

public:
    RouterTireTree();
    ~RouterTireTree();
    void AddRoute(const std::string& path, HttpHandler handler);
    bool HandleRequest(const std::string& path, HttpRequest& req, HttpResponse& res);
};

enum class MethodType {
    GET,
    POST,
    HEAD,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    PROPFIND,   // WebDAV相关
    PROPPATCH,  // WebDAV相关
    MKCOL,      // WebDAV相关
    COPY,       // WebDAV相关
    MOVE        // WebDAV相关
};

class IRouter{
public:
    virtual void AddRoute(MethodType method, const std::string& path, HttpHandler handler) = 0;
    virtual bool HandleRequest(const std::string &path, HttpRequest &req, HttpResponse &res) = 0;
};


class HttpRouter : public IRouter{
private:
    std::unordered_map<MethodType, RouterTireTree> tree_;

public:
    HttpRouter();
    void AddRoute(MethodType method, const std::string& path, HttpHandler handler) override;
    void Get(const std::string& path, HttpHandler handler);
    void Post(const std::string& path, HttpHandler handler);
    bool HandleRequest(const std::string &path, HttpRequest &req, HttpResponse &res) override;
    void StaticFile(const std::string& path);
};

}

#endif // HTTP_ROUTER_H_