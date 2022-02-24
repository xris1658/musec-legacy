#ifndef MUSEC_AUDIO_DRIVER_ASIODRIVERSTREAMINFO
#define MUSEC_AUDIO_DRIVER_ASIODRIVERSTREAMINFO

#include <common/asio.h>

#include <cstdint>

namespace Musec
{
namespace Audio
{
namespace Driver
{
struct ASIODriverStreamInfo
{
    long inputChannelCount;
    long outputChannelCount;
    long inputLatencyInSamples;
    long outputLatencyInSamples;
    long minimumBufferSize;
    long maximumBufferSize;
    long preferredBufferSize;
    long bufferSizeGranularity;
    ASIOSampleRate sampleRate;
};
}
}
}

#endif //MUSEC_AUDIO_DRIVER_ASIODRIVERSTREAMINFO
