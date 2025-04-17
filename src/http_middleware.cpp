#include "tiny_http/http_middleware.h"

namespace tiny_http{

void MiddlewareChain::Use(const Middleware& middleware){
    chain_->push_back(middleware);
}

void MiddlewareChain::Execute(HttpRequest &req, HttpResponse &res){
    const std::vector<Middleware>::const_iterator iter = chain_->cbegin();
    if (iter >= chain_->cend()){
        return;
    }

    // execute current middleware
    (*iter)(req, res, [this, &req, &res, iter]()
            { Execute(req, res, iter + 1); });
}

void MiddlewareChain::Execute(HttpRequest& req, HttpResponse& res, const std::vector<Middleware>::const_iterator iter){
    if(iter >= chain_->cend()){
        return;
    }

    // execute current middleware
    (*iter)(req, res, [this, &req, &res, iter](){
        Execute(req, res, iter + 1);
    });
}

}