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
constexpr char asioErrorOk[] = "操作成功完成。";
constexpr char asioErrorSuccess[]  = "操作成功完成。";
constexpr char asioErrorNotPresent[] = "没有硬件输入或输出设备，或者当前的设备不可用。";
constexpr char asioErrorHardwareMalfunction[] = "当前硬件没有正常工作。";
constexpr char asioErrorInvalidParameter[] = "当前使用的参数无效，可能需要检查音频驱动的设置。";
constexpr char asioErrorInvalidMode[] = "当前的硬件状态无效。";
constexpr char asioErrorSamplePositionIsNotAdvancing[] = "硬件无法返回采样时刻。";
constexpr char asioErrorNoClock[] = "硬件不支持当前使用的音频采样率。";
constexpr char asioErrorNoMemory[] = "内存空间不足，无法完成当前操作。";
constexpr char asioErrorUnknown[] = "未知错误。";

const char* asioErrorMessage(ASIOError error);
}
}
}
}

#endif //MUSEC_AUDIO_DRIVER_LITERALS
