#include "ThreadId.hpp"

namespace Musec
{
namespace Concurrent
{
std::thread::id mainThreadId_;
std::thread::id audioThreadId_;

std::thread::id mainThreadId()
{
    return mainThreadId_;
}

std::thread::id audioThreadId()
{
    return audioThreadId_;
}

void setMainThreadId(std::thread::id id)
{
    mainThreadId_ = id;
}

void setAudioThreadId(std::thread::id id)
{
    audioThreadId_ = id;
}
}
}