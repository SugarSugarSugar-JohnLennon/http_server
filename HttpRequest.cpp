#include "HttpRequest.h"
#include <sstream>
#include <algorithm>

HttpRequest::HttpRequest() : method_(UNKNOWN) {}

bool HttpRequest::parseRequest(const std::string& request) {
    std::istringstream iss(request);
    std::string line;
    if (!std::getline(iss, line)) return false;
    std::istringstream lineStream(line);
    std::string methodStr, pathStr;
    lineStream >> methodStr >> pathStr;
    if (methodStr == "GET") method_ = GET;
    else if (methodStr == "POST") method_ = POST;
    else method_ = UNKNOWN;
    path_ = pathStr;
    while (std::getline(iss, line) && line != "\r") {
        auto pos = line.find(":");
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            value.erase(0, value.find_first_not_of(" "));
            value.erase(std::remove(value.end()-2, value.end(), '\r'), value.end());
            headers_[key] = value;
        }
    }
    // 解析body
    std::string body;
    while (std::getline(iss, line)) {
        body += line + "\n";
    }
    if (!body.empty()) {
        if (body.back() == '\n') body.pop_back();
        body_ = body;
    }
    return true;
} 