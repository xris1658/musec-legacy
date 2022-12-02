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
template<std::size_t PPQ>
using MIDIClockNotifyFunc = void(*)(Musec::Audio::Base::TimePoint<PPQ>);

template<std::size_t PPQ>
class MIDIClock
{
public:
    MIDIClock(MIDIClockNotifyFunc<PPQ> notify, double tempo = 120.0):
        tempo_(tempo),
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
    double tempo()
    {
        return tempo_;
    }
    double setTempo(double tempo)
    {
        tempo_ = std::clamp(tempo, Musec::Audio::Base::minimumTempo, Musec::Audio::Base::maximumTempo);
        return tempo_;
    }
    Musec::Audio::Base::TempoAutomation<PPQ>& tempoAutomation()
    {
        return tempoAutomation_;
    }
    bool tempoAutomationInUse()
    {
        return !tempoAutomation_.empty();
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
        timePlayStarted_ = position_.count();
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
        auto fence = Musec::Native::currentTimePointInNanosecond();
        constexpr auto sleepRate = 0.95;
        while(!aboutToDie_)
        {
            if constexpr(AsyncNotify)
            {
                std::async(std::launch::async, notify_, position_);
            }
            else
            {
                notify_(position_);
            }
            auto spend = tempoAutomationInUse()?
                tempoAutomation_.secondElapsedInPulse(position_) * 1e9:
                Musec::Audio::Base::secondElapsed(tempo_, Musec::Audio::Base::Duration<PPQ>(1)) * 1e9;
            auto until = fence + std::chrono::nanoseconds(
                std::chrono::nanoseconds::rep(spend * sleepRate));
            fence += std::chrono::nanoseconds(
                std::chrono::nanoseconds::rep(spend));
            std::this_thread::sleep_until(until);
            // Hybrid sleep: Sleep for a while and wake up before the time is up,
            // then busy sleep until the time is up.
            // sleepRate indicates how long the thread should sleep for.
            while(Musec::Native::currentTimePointInNanosecond() < fence) { std::this_thread::yield(); }
            if(playing())
            {
                ++position_;
            }
        }
    }
public:
    std::thread& clockThread()
    {
        static std::thread ret(std::mem_fn(&MIDIClock<PPQ>::clockFunc<false>), this);
        return ret;
    }
private:
    double tempo_;
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
