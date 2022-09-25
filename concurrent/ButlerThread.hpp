#ifndef MUSEC_CONCURRENT_BUTLERTHREAD
#define MUSEC_CONCURRENT_BUTLERTHREAD

#include "base/CircularDeque.hpp"

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

namespace Musec
{
namespace Concurrent
{
enum class ButlerTaskType
{
    Stop,
    ResetASIODriver,
    ResizeASIOBuffer
};

class ButlerThread
{
public:
    ButlerThread();
    ButlerThread(const ButlerThread&) = delete;
    ButlerThread(ButlerThread&&) = delete;
    ~ButlerThread();
public:
    void butlerThreadFunc();
    void addTask(ButlerTaskType task);
public:
    static ButlerThread& instance();
private:
    std::thread butlerThread_;
    Musec::Base::CircularDeque<std::function<void()>, 8> taskQueue_;
    bool aboutToQuit_ = false;
    std::mutex mutex_;
    std::condition_variable_any cv_;
};
}
}

#endif //MUSEC_CONCURRENT_BUTLERTHREAD
