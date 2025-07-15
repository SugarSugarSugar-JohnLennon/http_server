#include "EventLoop.h"
#include "TcpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "utils.h"
#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <ctime>
#include "TcpConnection.h"
#include <iomanip>

// 简单日志宏
#define LOG(msg) do { \
    std::time_t t = std::time(nullptr); \
    std::cout << "[" << std::put_time(std::localtime(&t), "%F %T") << "] " << msg << std::endl; \
} while(0)

using HandlerFunc = std::function<void(const HttpRequest&, HttpResponse&)>;

std::map<std::string, HandlerFunc> g_routes;

void registerRoute(const std::string& path, HandlerFunc handler) {
    g_routes[path] = handler;
}

std::string getMimeType(const std::string& path) {
    if (path.size() >= 5 && path.substr(path.size()-5) == ".html") return "text/html";
    if (path.size() >= 4 && path.substr(path.size()-4) == ".css") return "text/css";
    if (path.size() >= 3 && path.substr(path.size()-3) == ".js") return "application/javascript";
    if (path.size() >= 4 && path.substr(path.size()-4) == ".png") return "image/png";
    if (path.size() >= 4 && path.substr(path.size()-4) == ".jpg") return "image/jpeg";
    return "text/plain";
}

void onHttpMessage(const std::shared_ptr<TcpServer>& server, const std::shared_ptr<TcpConnection>& conn, const std::string& data) {
    HttpRequest req;
    if (!req.parseRequest(data)) {
        HttpResponse resp;
        resp.setStatusCode(400);
        resp.setStatusMessage("Bad Request");
        resp.setBody("Bad Request");
        conn->send(resp.toString());
        LOG("400 Bad Request from fd=" << conn->fd());
        return;
    }
    HttpResponse resp;
    auto it = g_routes.find(req.path());
    if (it != g_routes.end()) {
        it->second(req, resp);
    } else if (req.method() == HttpRequest::GET) {
        std::string file = req.path();
        if (file == "/") file = "/index.html";
        std::string content = readFile("static" + file);
        if (!content.empty()) {
            resp.setStatusCode(200);
            resp.setStatusMessage("OK");
            resp.setContentType(getMimeType(file));
            resp.setBody(content);
        } else {
            resp.setStatusCode(404);
            resp.setStatusMessage("Not Found");
            resp.setBody("404 Not Found");
        }
    } else {
        resp.setStatusCode(405);
        resp.setStatusMessage("Method Not Allowed");
        resp.setBody("Method Not Allowed");
    }
    conn->send(resp.toString());
    LOG(req.path() << " " << req.method() << " fd=" << conn->fd());
}

int main() {
    EventLoop loop;
    int port = 8080;
    int numThreads = 4;
    auto server = std::make_shared<TcpServer>(&loop, port, numThreads);
    server->setMessageCallback([server](const std::shared_ptr<TcpConnection>& conn, const std::string& data) {
        onHttpMessage(server, conn, data);
    });

    // 注册路由
    registerRoute("/api/echo", [](const HttpRequest& req, HttpResponse& resp) {
        if (req.method() == HttpRequest::POST) {
            resp.setStatusCode(200);
            resp.setStatusMessage("OK");
            resp.setContentType("text/plain");
            resp.setBody("ECHO: " + req.body());
        } else {
            resp.setStatusCode(405);
            resp.setStatusMessage("Method Not Allowed");
            resp.setBody("Only POST allowed");
        }
    });

    registerRoute("/api/hello", [](const HttpRequest& req, HttpResponse& resp) {
        resp.setStatusCode(200);
        resp.setStatusMessage("OK");
        resp.setContentType("application/json");
        resp.setBody("{\"msg\":\"hello, world!\"}");
    });

    server->start();
    LOG("HTTP server started on port " << port);
    loop.loop();
    return 0;
} 