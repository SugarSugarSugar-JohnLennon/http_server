#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <memory>
#include <atomic>

class EventLoop;

class ThreadPool {
public:
    ThreadPool(int numThreads);
    ~ThreadPool();

    void start();
    EventLoop* getNextLoop();

private:
    int numThreads_;
    std::vector<std::thread> threads_;
    std::vector<std::unique_ptr<EventLoop>> loops_;
    std::atomic<int> next_;
    bool started_;
}; 