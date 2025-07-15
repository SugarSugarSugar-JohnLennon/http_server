#include "EventLoop.h"
#include "Channel.h"
#include <unistd.h>
#include <sys/eventfd.h>
#include <cassert>
#include <iostream>

EventLoop::EventLoop()
    : epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
      wakeupFd_(::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)),
      looping_(false),
      quit_(false),
      events_(1024),
      threadId_(::getpid())
{
    assert(epollfd_ >= 0);
    assert(wakeupFd_ >= 0);
    // TODO: 将wakeupFd_加入epoll监听
}

EventLoop::~EventLoop() {
    ::close(epollfd_);
    ::close(wakeupFd_);
}

void EventLoop::loop() {
    looping_ = true;
    while (!quit_) {
        int numEvents = ::epoll_wait(epollfd_, events_.data(), static_cast<int>(events_.size()), -1);
        for (int i = 0; i < numEvents; ++i) {
            Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
            channel->handleEvent();
        }
        doPendingFunctors();
    }
    looping_ = false;
}

void EventLoop::quit() {
    quit_ = true;
    // TODO: 唤醒loop
}

void EventLoop::runInLoop(Functor cb) {
    if (isInLoopThread()) {
        cb();
    } else {
        queueInLoop(std::move(cb));
    }
}

void EventLoop::queueInLoop(Functor cb) {
    pendingFunctors_.push_back(std::move(cb));
    // TODO: 唤醒loop
}

void EventLoop::updateChannel(Channel* channel) {
    // TODO: 实现epoll_ctl添加/修改channel
}

void EventLoop::removeChannel(Channel* channel) {
    // TODO: 实现epoll_ctl删除channel
}

bool EventLoop::isInLoopThread() const {
    return threadId_ == ::getpid();
}

void EventLoop::handleWakeup() {
    uint64_t one = 1;
    ::read(wakeupFd_, &one, sizeof one);
}

void EventLoop::doPendingFunctors() {
    std::vector<Functor> functors;
    functors.swap(pendingFunctors_);
    for (const Functor& func : functors) {
        func();
    }
} 