#pragma once
#include <string>
#include <unordered_map>

class HttpResponse {
public:
    HttpResponse();
    void setStatusCode(int code) { statusCode_ = code; }
    void setStatusMessage(const std::string& msg) { statusMessage_ = msg; }
    void setContentType(const std::string& type) { contentType_ = type; }
    void setBody(const std::string& body) { body_ = body; }
    void addHeader(const std::string& key, const std::string& value) { headers_[key] = value; }
    std::string toString() const;
private:
    int statusCode_;
    std::string statusMessage_;
    std::string contentType_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
}; 