#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include <string>

class EventLoop;
class Acceptor;
class ThreadPool;
class TcpConnection;

class TcpServer {
public:
    using MessageCallback = std::function<void(const std::shared_ptr<TcpConnection>&, const std::string&)>;

    TcpServer(EventLoop* loop, int port, int numThreads);
    ~TcpServer();

    void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }
    void start();

private:
    void newConnection(int sockfd);
    void removeConnection(const std::shared_ptr<TcpConnection>& conn);

    EventLoop* loop_;
    std::unique_ptr<Acceptor> acceptor_;
    std::unique_ptr<ThreadPool> threadPool_;
    std::unordered_map<int, std::shared_ptr<TcpConnection>> connections_;
    MessageCallback messageCallback_;
    int nextConnId_;
}; 