#include "ThreadPool.hpp"

namespace Musec::Concurrent
{
ThreadPool::ThreadPool(std::size_t fallbackThreadCount)
{
    // Windows + MSVC returns the logic processor count (that count you see in the task manager)
    // Not implemented in some older versions of GCC
    auto threadCount = std::thread::hardware_concurrency();
    if(!threadCount)
    {
        threadCount = Musec::Native::getProcessCPUCoreCount();
        if(!threadCount)
        {
            threadCount = fallbackThreadCount? fallbackThreadCount: 8;
        }
    }
    threads_.reserve(threadCount);
    for(int i = 0; i < threadCount; ++i)
    {
        threads_.emplace_back();
    }
}

ThreadPool::~ThreadPool()
{
    for(auto& thread: threads_)
    {
        if(thread.joinable())
        {
            thread.detach();
        }
    }
}

std::size_t ThreadPool::threadCount() const noexcept
{
    return threads_.size();
}

const std::thread& ThreadPool::operator[](std::size_t index) const
{
    return threads_[index];
}

std::thread& ThreadPool::operator[](std::size_t index)
{
    return threads_[index];
}

const std::thread& ThreadPool::at(std::size_t index) const
{
    return threads_.at(index);
}

std::thread& ThreadPool::at(std::size_t index)
{
    return threads_.at(index);
}
}