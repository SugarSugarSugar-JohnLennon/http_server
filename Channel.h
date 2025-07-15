#pragma once
#include <functional>
#include <sys/epoll.h>

class EventLoop;

class Channel {
public:
    using EventCallback = std::function<void()>;
    Channel(EventLoop* loop, int fd);
    ~Channel();

    void handleEvent();
    void setReadCallback(EventCallback cb) { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

    int fd() const { return fd_; }
    uint32_t events() const { return events_; }
    void setEvents(uint32_t ev) { events_ = ev; }
    uint32_t revents() const { return revents_; }
    void setRevents(uint32_t rev) { revents_ = rev; }

    void enableReading();
    void enableWriting();
    void disableWriting();
    void disableAll();

    EventLoop* ownerLoop() { return loop_; }

private:
    void update();

    EventLoop* loop_;
    const int fd_;
    uint32_t events_;
    uint32_t revents_;
    bool addedToLoop_;
    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
}; 