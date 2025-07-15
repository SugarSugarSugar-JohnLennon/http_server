#include "ThreadPool.h"
#include "EventLoop.h"
#include <cassert>

ThreadPool::ThreadPool(int numThreads)
    : numThreads_(numThreads), next_(0), started_(false) {}

ThreadPool::~ThreadPool() {}

void ThreadPool::start() {
    assert(!started_);
    started_ = true;
    for (int i = 0; i < numThreads_; ++i) {
        loops_.emplace_back(new EventLoop());
        threads_.emplace_back([this, i]() {
            loops_[i]->loop();
        });
    }
}

EventLoop* ThreadPool::getNextLoop() {
    if (loops_.empty()) return nullptr;
    int idx = next_++ % numThreads_;
    return loops_[idx].get();
} 