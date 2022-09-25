#include "ButlerThread.hpp"

#include "audio/driver/ASIODriver.hpp"
#include "controller/ASIODriverController.hpp"

namespace Musec::Concurrent
{
namespace Impl
{
template<ButlerTaskType Task>
void doTask();

template<>
void doTask<ButlerTaskType::ResetASIODriver>()
{
    using namespace Musec::Audio::Driver;
    auto info = AppASIODriver().driverInfo();
    Musec::Controller::ASIODriverController::setASIODriver(std::get<1>(info));
}

template<>
void doTask<ButlerTaskType::ResizeASIOBuffer>()
{
    using namespace Musec::Audio::Driver;
    using namespace Musec::Controller::ASIODriverController;
    AppASIODriver()->stop();
    AppASIODriver()->disposeBuffers();
    allocateASIODriverBuffer();
    startASIODriver();
}
}

ButlerThread::ButlerThread():
    butlerThread_(std::mem_fn(&ButlerThread::butlerThreadFunc), this),
    taskQueue_(),
    aboutToQuit_(false),
    mutex_(),
    cv_()
{
    //
}

ButlerThread::~ButlerThread()
{
    addTask(ButlerTaskType::Stop);
    butlerThread_.join();
}

void ButlerThread::butlerThreadFunc()
{
    while(!aboutToQuit_)
    {
        std::lock_guard<std::mutex> lg(mutex_);
        cv_.wait(mutex_, [this]() { return (!taskQueue_.empty()) || aboutToQuit_; });
        auto size = taskQueue_.size();
        for(decltype(size) i = 0; i < size; ++i)
        {
            taskQueue_.front()();
            taskQueue_.popFront();
        }
    }
}

void ButlerThread::addTask(ButlerTaskType task)
{
    std::lock_guard<std::mutex> lg(mutex_);
    if(task == ButlerTaskType::Stop)
    {
        aboutToQuit_ = true;
        cv_.notify_one();
    }
    else if(task == ButlerTaskType::ResetASIODriver)
    {
        taskQueue_.pushBack(std::function<void()>(Impl::doTask<ButlerTaskType::ResetASIODriver>));
        cv_.notify_one();
    }
    else if(task == ButlerTaskType::ResizeASIOBuffer)
    {
        taskQueue_.pushBack(std::function<void()>(Impl::doTask<ButlerTaskType::ResizeASIOBuffer>));
        cv_.notify_one();
    }
}

ButlerThread& ButlerThread::instance()
{
    static ButlerThread ret;
    return ret;
}
}