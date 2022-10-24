#ifndef MUSEC_AUDIO_DRIVER_LITERALS
#define MUSEC_AUDIO_DRIVER_LITERALS

#include "common/asio.h"

#include <QString>

namespace Musec
{
namespace Audio
{
namespace Driver
{
namespace Literals
{
constexpr char asioErrorOk[] = "asioErrorOkText";
constexpr char asioErrorSuccess[]  ="asioErrorSuccessText";
constexpr char asioErrorNotPresent[] = "asioErrorNotPresentText";
constexpr char asioErrorHardwareMalfunction[] = "asioErrorHardwareMalfunctionText";
constexpr char asioErrorInvalidParameter[] = "asioErrorInvalidParameterText";
constexpr char asioErrorInvalidMode[] = "asioErrorInvalidModeText";
constexpr char asioErrorSamplePositionIsNotAdvancing[] = "asioErrorSamplePositionIsNotAdvancingText";
constexpr char asioErrorNoClock[] = "asioErrorNoClockText";
constexpr char asioErrorNoMemory[] = "asioErrorNoMemoryText";
constexpr char asioErrorUnknown[] = "asioErrorUnknownText";

QString asioErrorMessage(ASIOError error);
}
}
}
}

#endif //MUSEC_AUDIO_DRIVER_LITERALS
