#pragma once
#include <string>
#include <unordered_map>

class HttpRequest {
public:
    enum Method { GET, POST, UNKNOWN };
    HttpRequest();
    bool parseRequest(const std::string& request);
    Method method() const { return method_; }
    const std::string& path() const { return path_; }
    const std::unordered_map<std::string, std::string>& headers() const { return headers_; }
    const std::string& body() const { return body_; }
private:
    Method method_;
    std::string path_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
}; 