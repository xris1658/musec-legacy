#include "ModuleConnector.hpp"

#include "audio/driver/ASIODriver.hpp"
#include "audio/driver/ASIODriverStreamInfo.hpp"
#include "audio/driver/ASIOCallback.hpp"
#include "event/EventBase.hpp"
#include "ui/UI.hpp"

namespace Musec
{
void connectFacility()
{
Musec::Audio::Driver::functionThatUsesDuration =
    [](std::int64_t durationInNanoSecond)
    {
        auto streamInfo = Musec::Audio::Driver::getASIODriverStreamInfo();
        double bufferTimeInNanosecond = static_cast<double>(streamInfo.preferredBufferSize)
            / static_cast<double>(streamInfo.sampleRate) * 1e9;
        double usage = durationInNanoSecond / bufferTimeInNanosecond;
        Musec::Event::eventHandler->updateUsage(usage);
    };
}
}