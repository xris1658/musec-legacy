#include "ThreadPool.hpp"

namespace Musec::Concurrent
{
ThreadPool::ThreadPool(std::size_t fallbackThreadCount)
{
    // 标准库中的设施
    // Windows + MSVC：返回逻辑处理器数
    // 部分旧版本的 GCC 没有实现这一函数
    auto threadCount = std::thread::hardware_concurrency();
    if(!threadCount)
    {
        // Native 措施（Windows 中查看进程相关性，得出可使用的逻辑处理器数）
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