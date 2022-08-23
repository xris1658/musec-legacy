#ifndef MUSEC_AUDIO_ENGINE_MIDICLOCK
#define MUSEC_AUDIO_ENGINE_MIDICLOCK

#include "audio/base/TempoAutomation.hpp"
#include "audio/base/Chrono.hpp"
#include "native/Native.hpp"

#include <functional>
#include <future>
#include <thread>

namespace Musec
{
namespace Audio
{
namespace Engine
{
namespace Impl
{
std::int64_t getCurrentTimeInNanosecond();
}
template<std::size_t PPQ>
using MIDIClockNotifyFunc = void(*)(Musec::Audio::Base::TimePoint<PPQ>);

template<std::size_t PPQ>
class MIDIClock
{
public:
    MIDIClock(MIDIClockNotifyFunc<PPQ> notify):
        tempoAutomation_(),
        position_(0),
        playing_(false),
        timePlayStarted_(0),
        aboutToDie_(false),
        notify_(notify)
    {
    }
    ~MIDIClock()
    {
        aboutToDie_ = true;
    }
public:
    Musec::Audio::Base::TempoAutomation<PPQ>& tempoAutomation()
    {
        return tempoAutomation_;
    }
    void setPosition(Musec::Audio::Base::TimePoint<PPQ> position)
    {
        position_ = position;
    }
    Musec::Audio::Base::TimePoint<PPQ> getPosition() const
    {
        return position_;
    }
public:
    bool playing() const noexcept
    {
        return playing_;
    }
    void play()
    {
        timePlayStarted_ = position_;
        playing_ = true;
    }
    void stop()
    {
        playing_ = false;
    }
public:
    template<bool AsyncNotify = true>
    void clockFunc()
    {
        Musec::Native::setThreadPriorityToTimeCritical();
        while(!aboutToDie_)
        {
            while(!playing_) { Sleep(0); }
            auto fence = Impl::getCurrentTimeInNanosecond();
            while(playing_)
            {
                if(AsyncNotify)
                {
                    std::async(std::launch::async, notify_, position_);
                }
                else
                {
                    notify_(position_);
                }
                auto delta = tempoAutomation_.secondElapsedInPulse(position_) * 1e9;
                fence += delta;
                while(Impl::getCurrentTimeInNanosecond() < fence) { Sleep(0); }
                ++position_;
            }
        }
    }
public:
    std::thread& clockThread()
    {
        static std::thread ret(std::mem_fn(&MIDIClock<PPQ>::clockFunc), this);
        return ret;
    }
private:
    Musec::Audio::Base::TempoAutomation<PPQ> tempoAutomation_;
    Musec::Audio::Base::TimePoint<PPQ> position_;
    bool playing_;
    std::int64_t timePlayStarted_;
    bool aboutToDie_;
    MIDIClockNotifyFunc<PPQ> notify_;
};
}
}
}


#endif //MUSEC_AUDIO_ENGINE_MIDICLOCK
