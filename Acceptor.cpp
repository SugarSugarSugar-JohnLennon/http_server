#include "Acceptor.h"
#include "EventLoop.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cassert>
#include <iostream>

static int createNonblockingOrDie(int port) {
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    assert(sockfd >= 0);
    struct sockaddr_in addr;
    ::memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    int opt = 1;
    ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
    int ret = ::bind(sockfd, (struct sockaddr*)&addr, sizeof addr);
    assert(ret >= 0);
    return sockfd;
}

Acceptor::Acceptor(EventLoop* loop, int port)
    : loop_(loop), listenfd_(createNonblockingOrDie(port)), port_(port), listening_(false) {}

Acceptor::~Acceptor() {
    ::close(listenfd_);
}

void Acceptor::listen() {
    listening_ = true;
    ::listen(listenfd_, SOMAXCONN);
    // TODO: 将listenfd_加入EventLoop监听
}

void Acceptor::handleRead() {
    struct sockaddr_in addr;
    socklen_t len = sizeof addr;
    int connfd = ::accept4(listenfd_, (struct sockaddr*)&addr, &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd >= 0) {
        if (newConnectionCallback_) {
            newConnectionCallback_(connfd);
        } else {
            ::close(connfd);
        }
    } else {
        std::cerr << "Acceptor::handleRead() accept error" << std::endl;
    }
} 