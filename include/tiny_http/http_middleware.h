#ifndef HTTP_MIDDLEWARE_H_
#define HTTP_MIDDLEWARE_H_

#include "tiny_http/http_request.h"
#include <functional>
#include <vector>
#include <memory>

namespace tiny_http{
using Middleware = std::function<void(HttpRequest&, HttpResponse&, const std::function<void()>&)>;

class MiddlewareChain{
public:
    MiddlewareChain():chain_(std::make_unique<std::vector<Middleware> >()){}

public:
    void Use(const Middleware& middleware);
    void Execute(HttpRequest &req, HttpResponse &res);
    void Execute(HttpRequest &req, HttpResponse &res, std::vector<Middleware>::const_iterator iter);

private:
    std::unique_ptr<std::vector<Middleware> > chain_;
};

}

#endif