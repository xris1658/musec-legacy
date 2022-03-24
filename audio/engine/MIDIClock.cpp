#include "MIDIClock.hpp"

namespace Musec::Audio::Engine
{
namespace Impl
{
std::int64_t getCurrentTimeInNanosecond()
{
auto oldMask = Musec::Native::setThreadMask(Musec::Native::getMIDIClockThreadAffinity());
auto ret = Musec::Native::currentTimeInNanosecond();
Musec::Native::setThreadMask(oldMask);
return ret;
//return Musec::Native::currentTimeInNanosecond();
}
}
}