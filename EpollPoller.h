#pragma once
#include <vector>
#include <sys/epoll.h>

class Channel;

class EpollPoller {
public:
    EpollPoller();
    ~EpollPoller();

    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    int poll(int timeoutMs, std::vector<Channel*>& activeChannels);

private:
    int epollfd_;
    std::vector<struct epoll_event> events_;
}; 