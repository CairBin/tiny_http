# tiny_http

## 描述

基于C++17标准实现的简易http服务器，支持路由。

## 使用方式

编译项目

```sh
make clean && make all
```

在`./lib`目录下会生成静态库文件，将它和头文件导入的你的程序中，下面是一个样例：

```cpp
#include <iostream>
#include "tiny_http/http_server.h"
#include "tiny_http/http_router.h"
int main(){
    // 设置路由
    auto router = std::make_unique<tiny_http::HttpRouter>();
    router->Get("/", [](tiny_http::HttpRequest& req, tiny_http::HttpResponse& res){
        res.SetStatus(404).Send("<h1>HelloWorld</h1>");
    });
    std::unique_ptr<tiny_http::IRouter> irouter = std::move(router);

    // 初始化Http服务器
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
```

## 路由

`HttpRouter`类继承自`IRouter`类，`IRouter`声明了两个虚方法：

```cpp
virtual void AddRoute(MethodType method, const std::string& path, HttpHandler handler) = 0;
virtual bool HandleRequest(const std::string &path, HttpRequest &req, HttpResponse &res) = 0;
```

`AddRoute`用于添加`MethodType`枚举类任意声明的Http请求方法（第一个参数），其第二个参数表示Http请求路径，第三个参数为`HttpHandler`类型，它是用于处理Http请求的回调函数。关于`HttpHandler`的定义如下：

```cpp
typedef void(*HttpHandler)(HttpRequest& req, HttpResponse& res);
```

对于`MethodType`所声明的Http1.1请求方法类型有如下几种：

```cpp
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
```

`HttpRouter`提供`Get`和`Post`方法便于快速构建路径对应的请求，其声明如下：

```cpp
void Get(const std::string& path, HttpHandler handler);
void Post(const std::string& path, HttpHandler handler);
```

实际上就是在内部调用了`AddRoute`方法

```cpp
void HttpRouter::Get(const std::string& path, HttpHandler handler){
    AddRoute(MethodType::GET, path, handler);
}
```

`HttpServer`类所需路由参数为`IRouter`的智能指针的引用，这就意味着你可以在外部封装自己的路由组件（继承自`IRouter`）来传递给`HttpServer`，而不必须使用提供的`HttpRouter`。通过这个特性，你可以仿照`HttpRouter`的`Get`和`Post`方法来实现其它Http请求方法对应的路由方法，以及在此基础上实现你的框架。

```cpp
class MyRouter : public tiny_http::IRouter{
    [...]
    void Put(const std::string& path, HttpHandler handler){
        AddRoute(MethodType::PUT, path, handler);
    }
};

[...]

int main(){
    auto router = std::make_unique<MyRouter>();
    router->Put("/", [](tiny_http::HttpRequest& req, tiny_http::HttpResponse& res){
        [...]
    });
    std::unique_ptr<tiny_http::IRouter> irouter = std::move(router);
    tiny_http::HttpServer server(8080, std::ref(irouter));
    [...]
}
```

## 贡献&许可证

* 本项目遵循MIT许可证
* 您如果想提交代码至本项目请遵循Google C++命名规范