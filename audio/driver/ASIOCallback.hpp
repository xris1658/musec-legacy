#ifndef MUSEC_AUDIO_DRIVER_ASIOCALLBACK
#define MUSEC_AUDIO_DRIVER_ASIOCALLBACK

#include "IASIODriver.hpp"

namespace Musec
{
namespace Audio
{
namespace Driver
{
void onASIOBufferSwitch(long doubleBufferIndex,
                        ASIOBool directProcess);

ASIOTime* onASIOBufferSwitchTimeInfo(ASIOTime* params,
                                     long doubleBufferIndex,
                                     ASIOBool directProcess);

void onASIOSampleRateDidChange(ASIOSampleRate sRate);

long onASIOMessage(long selector,
                   long value,
                   void* message,
                   double* opt);

ASIOCallbacks& getCallbacks();
}
}
}

#endif // MUSEC_AUDIO_DRIVER_ASIOCALLBACK
