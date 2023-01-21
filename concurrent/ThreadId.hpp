#ifndef MUSEC_CONCURRENT_THREADID
#define MUSEC_CONCURRENT_THREADID

#include <thread>

namespace Musec
{
namespace Concurrent
{
std::thread::id mainThreadId();
std::thread::id audioThreadId();
void setMainThreadId(std::thread::id id);
void setAudioThreadId(std::thread::id id);
}
}

#endif //MUSEC_CONCURRENT_THREADID
