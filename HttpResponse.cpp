#include "HttpResponse.h"
#include <sstream>

HttpResponse::HttpResponse() : statusCode_(200), statusMessage_("OK"), contentType_("text/html") {}

std::string HttpResponse::toString() const {
    std::ostringstream oss;
    oss << "HTTP/1.1 " << statusCode_ << " " << statusMessage_ << "\r\n";
    oss << "Content-Type: " << contentType_ << "\r\n";
    for (const auto& kv : headers_) {
        oss << kv.first << ": " << kv.second << "\r\n";
    }
    oss << "Content-Length: " << body_.size() << "\r\n";
    oss << "\r\n";
    oss << body_;
    return oss.str();
} 