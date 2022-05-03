#ifndef MUSEC_DKWTP_THREADPOOL
#define MUSEC_DKWTP_THREADPOOL

#include "native/Native.hpp"

#include <thread>
#include <vector>

namespace Musec
{
namespace Concurrent
{
class ThreadPool
{
public:
    ThreadPool(std::size_t fallbackThreadCount);
    ~ThreadPool();
public:
    std::size_t threadCount() const noexcept;
    const std::thread& operator[](std::size_t index) const;
    std::thread& operator[](std::size_t index);
    const std::thread& at(std::size_t index) const;
    std::thread& at(std::size_t index);
private:
    std::vector<std::thread> threads_;
};
}
}


#endif //MUSEC_DKWTP_THREADPOOL
