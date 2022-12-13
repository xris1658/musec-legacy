#ifndef MUSEC_AUDIO_DRIVER_ASIOCALLBACK
#define MUSEC_AUDIO_DRIVER_ASIOCALLBACK

#include "ASIODriverForward.hpp"

#include <cstdint>

namespace Musec
{
namespace Audio
{
namespace Driver
{
extern double cpuUsage;

extern bool driverSupportsOutputReady;

ASIOCallbacks& getCallbacks();
}
}
}

#endif // MUSEC_AUDIO_DRIVER_ASIOCALLBACK
