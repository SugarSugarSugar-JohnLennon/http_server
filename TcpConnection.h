#pragma once
#include <functional>
#include <memory>
#include <string>

class EventLoop;
class Channel;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    using MessageCallback = std::function<void(const std::shared_ptr<TcpConnection>&, const std::string&)>;
    using CloseCallback = std::function<void(const std::shared_ptr<TcpConnection>&)>;

    TcpConnection(EventLoop* loop, int sockfd);
    ~TcpConnection();

    void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }
    void setCloseCallback(const CloseCallback& cb) { closeCallback_ = cb; }

    void send(const std::string& msg);
    void shutdown();

    int fd() const { return sockfd_; }

private:
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();

    EventLoop* loop_;
    int sockfd_;
    std::unique_ptr<Channel> channel_;
    MessageCallback messageCallback_;
    CloseCallback closeCallback_;
    std::string readBuffer_;
    std::string writeBuffer_;
}; 