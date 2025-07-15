#include "EpollPoller.h"
#include "Channel.h"
#include <unistd.h>
#include <cassert>
#include <cstring>

EpollPoller::EpollPoller()
    : epollfd_(::epoll_create1(EPOLL_CLOEXEC)), events_(1024) {
    assert(epollfd_ >= 0);
}

EpollPoller::~EpollPoller() {
    ::close(epollfd_);
}

void EpollPoller::updateChannel(Channel* channel) {
    struct epoll_event ev = {0};
    ev.events = channel->events();
    ev.data.ptr = channel;
    ::epoll_ctl(epollfd_, EPOLL_CTL_ADD, channel->fd(), &ev); // 简化处理，实际应区分ADD/MOD
}

void EpollPoller::removeChannel(Channel* channel) {
    ::epoll_ctl(epollfd_, EPOLL_CTL_DEL, channel->fd(), nullptr);
}

int EpollPoller::poll(int timeoutMs, std::vector<Channel*>& activeChannels) {
    int numEvents = ::epoll_wait(epollfd_, events_.data(), static_cast<int>(events_.size()), timeoutMs);
    if (numEvents > 0) {
        for (int i = 0; i < numEvents; ++i) {
            Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
            channel->setRevents(events_[i].events);
            activeChannels.push_back(channel);
        }
    }
    return numEvents;
} 