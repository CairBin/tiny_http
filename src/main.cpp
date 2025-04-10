/*
 * 测试文件
*/
#include <iostream>
#include "tiny_http/http_server.h"
#include "tiny_http/http_router.h"
int main(){
    auto router = std::make_unique<tiny_http::HttpRouter>();
    router->Get("/", [](tiny_http::HttpRequest& req, tiny_http::HttpResponse& res){
        res.SetStatus(404).Send("<h1>HelloWorld</h1>");
    });

    router->Get("/test/:id/:name", [](tiny_http::HttpRequest& req, tiny_http::HttpResponse& res){
        std::string html = "";
        for(auto &param : req.params_){
            html += "<h1>" + param.first + ": " + param.second + "</h1>";
        }
        html += "<script>console.log(123);</script>";
        res.SetStatus(200).Send(html);
    });

    std::unique_ptr<tiny_http::IRouter> irouter = std::move(router);

    tiny_http::HttpServer server(8080, std::ref(irouter));
    if(server.Initialize() < 0){
        std::cerr << "Initialize failed" << std::endl;
        return -1;
    }
    if(server.Bind() < 0){
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    std::cout << "Server started on port "
        << server.get_port() 
        << "..."
        << std::endl;

    if(server.Listen() < 0 ){
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }
    
    
    return 0;
}