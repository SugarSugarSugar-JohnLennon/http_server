#include "TcpServer.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "ThreadPool.h"
#include "TcpConnection.h"
#include <cassert>
#include <iostream>

TcpServer::TcpServer(EventLoop* loop, int port, int numThreads)
    : loop_(loop),
      acceptor_(new Acceptor(loop, port)),
      threadPool_(new ThreadPool(numThreads)),
      nextConnId_(1) {
    acceptor_->setNewConnectionCallback([this](int sockfd) { newConnection(sockfd); });
}

TcpServer::~TcpServer() {}

void TcpServer::start() {
    threadPool_->start();
    acceptor_->listen();
}

void TcpServer::newConnection(int sockfd) {
    EventLoop* ioLoop = threadPool_->getNextLoop();
    if (!ioLoop) ioLoop = loop_;
    auto conn = std::make_shared<TcpConnection>(ioLoop, sockfd);
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallback([this](const std::shared_ptr<TcpConnection>& c) { removeConnection(c); });
    connections_[nextConnId_++] = conn;
}

void TcpServer::removeConnection(const std::shared_ptr<TcpConnection>& conn) {
    connections_.erase(conn->fd());
} 