#pragma once
#include <vector>
#include <functional>
#include <atomic>
#include <memory>
#include <sys/epoll.h>

class Channel;

class EventLoop {
public:
    using Functor = std::function<void()>;
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();
    void runInLoop(Functor cb);
    void queueInLoop(Functor cb);

    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    bool isInLoopThread() const;

private:
    void handleWakeup();
    void doPendingFunctors();

    int epollfd_;
    int wakeupFd_;
    std::atomic<bool> looping_;
    std::atomic<bool> quit_;
    std::vector<struct epoll_event> events_;
    std::vector<Functor> pendingFunctors_;
    pid_t threadId_;
}; 