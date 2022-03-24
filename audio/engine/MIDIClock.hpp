#ifndef MUSEC_AUDIO_ENGINE_MIDICLOCK
#define MUSEC_AUDIO_ENGINE_MIDICLOCK

#include "audio/base/TempoAutomationWithCurve.hpp"
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
    Musec::Audio::Base::TempoAutomationWithCurve<PPQ>& tempoAutomation()
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
    void clockFunc()
    {
        Musec::Native::setThreadPriorityToTimeCritical();
        while(!aboutToDie_)
        {
            while(!playing_) {}
            auto fence = Impl::getCurrentTimeInNanosecond();
            while(playing_)
            {
                 // notify_ 可能相对费时，因此异步调用
                 std::async(std::launch::async, notify_, position_);
                auto delta = tempoAutomation_.secondElapsedInPulse(position_) * 1e9;
                fence += delta;
                while(Impl::getCurrentTimeInNanosecond() < fence) {}
                ++position_;
            }
        }
    }
private:
    // 速度自动化
    Musec::Audio::Base::TempoAutomationWithCurve<PPQ> tempoAutomation_;
    // 当前所在位置（PPQ）
    Musec::Audio::Base::TimePoint<PPQ> position_;
    // 是否正在播放
    bool playing_;
    // 运行播放函数时所在位置（PPQ）
    std::int64_t timePlayStarted_;
    bool aboutToDie_;
    MIDIClockNotifyFunc<PPQ> notify_;
};
}
}
}


#endif //MUSEC_AUDIO_ENGINE_MIDICLOCK
