#pragma once
#include <functional>

class EventLoop;

class Acceptor {
public:
    using NewConnectionCallback = std::function<void(int sockfd)>;
    Acceptor(EventLoop* loop, int port);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback& cb) { newConnectionCallback_ = cb; }
    void listen();

private:
    void handleRead();

    EventLoop* loop_;
    int listenfd_;
    int port_;
    NewConnectionCallback newConnectionCallback_;
    bool listening_;
}; 