#include "TcpConnection.h"
#include "EventLoop.h"
#include "Channel.h"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>

TcpConnection::TcpConnection(EventLoop* loop, int sockfd)
    : loop_(loop), sockfd_(sockfd), channel_(new Channel(loop, sockfd)) {
    channel_->setReadCallback([this]() { handleRead(); });
    channel_->setWriteCallback([this]() { handleWrite(); });
    channel_->setCloseCallback([this]() { handleClose(); });
    channel_->setErrorCallback([this]() { handleError(); });
    channel_->enableReading();
}

TcpConnection::~TcpConnection() {
    ::close(sockfd_);
}

void TcpConnection::send(const std::string& msg) {
    writeBuffer_ += msg;
    channel_->enableWriting();
}

void TcpConnection::shutdown() {
    ::shutdown(sockfd_, SHUT_WR);
}

void TcpConnection::handleRead() {
    char buf[4096];
    ssize_t n = ::read(sockfd_, buf, sizeof buf);
    if (n > 0) {
        readBuffer_.append(buf, n);
        if (messageCallback_) {
            messageCallback_(shared_from_this(), readBuffer_);
            readBuffer_.clear();
        }
    } else if (n == 0) {
        handleClose();
    } else {
        handleError();
    }
}

void TcpConnection::handleWrite() {
    ssize_t n = ::write(sockfd_, writeBuffer_.data(), writeBuffer_.size());
    if (n > 0) {
        writeBuffer_ = writeBuffer_.substr(n);
        if (writeBuffer_.empty()) {
            // 关闭写事件
            channel_->disableWriting();
        }
    } else {
        handleError();
    }
}

void TcpConnection::handleClose() {
    if (closeCallback_) {
        closeCallback_(shared_from_this());
    }
}

void TcpConnection::handleError() {
    std::cerr << "TcpConnection::handleError() fd=" << sockfd_ << std::endl;
    handleClose();
} 