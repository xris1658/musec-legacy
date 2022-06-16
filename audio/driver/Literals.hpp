#ifndef MUSEC_AUDIO_DRIVER_LITERALS
#define MUSEC_AUDIO_DRIVER_LITERALS

#include "common/asio.h"

#include <QChar>

namespace Musec
{
namespace Audio
{
namespace Driver
{
namespace Literals
{
constexpr char asioErrorOk[] = "The operation is done succesfully.";
constexpr char asioErrorSuccess[]  = "The operation is done succesfully.";
constexpr char asioErrorNotPresent[] = "There is no hardware input or output devices, or the devices are not available.";
constexpr char asioErrorHardwareMalfunction[] = "The current hardware is malfunctioning.";
constexpr char asioErrorInvalidParameter[] = "The current parameter is invalid. You might need to check settings of the audio driver.";
constexpr char asioErrorInvalidMode[] = "The mode of the hardware is invalid.";
constexpr char asioErrorSamplePositionIsNotAdvancing[] = "The hardware can't return the sample position.";
constexpr char asioErrorNoClock[] = "The sample rate selected is not supported by the hardware.";
constexpr char asioErrorNoMemory[] = "The available memory space is not enough to complete current operation.";
constexpr char asioErrorUnknown[] = "Unknown error. Unfortunately, that's all we know.";

const char* asioErrorMessage(ASIOError error);
}
}
}
}

#endif //MUSEC_AUDIO_DRIVER_LITERALS
