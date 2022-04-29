#ifndef MUSEC_NATIVE_NATIVE
#define MUSEC_NATIVE_NATIVE

#include "util/Endian.hpp"

#include <QString>

#include <Windows.h>

#include <array>

namespace Musec
{
namespace Native
{
using SystemTimeType = SYSTEMTIME;

using ThreadMaskType = std::uint64_t;

using SystemTimeStringType = std::array<char, 18>;

using WindowType = HWND;

using ErrorCodeType = decltype(GetLastError());
// 漫游用户数据路径（用户文件夹\AppData\Roaming）
const QString& RoamingDirectoryPath();

// Musec 程序用户数据路径（用户文件夹\AppData\Roaming\Musec）
const QString& DataDirectoryPath();

SystemTimeType getCurrentTime();

SystemTimeType getLaunchTime();

// 获取系统时间类型，并转换为YYYYMMDDHHMMSS 格式的字符串
SystemTimeStringType formatTime(const SystemTimeType& time);

void openSpecialCharacterInput();

void showFileInExplorer(const QString& path);

int getProcessCPUCoreCount();

ThreadMaskType getMIDIClockThreadAffinity();

ThreadMaskType setThreadMask(ThreadMaskType mask);

// 获取高精度时间
std::int64_t currentTimeInNanosecond();

void setThreadPriorityToTimeCritical();

constexpr Musec::Util::Endian endian();

// 检测应用自身是否处于调试状态。目前仅用于辅助选择文字渲染。
bool isDebuggerPresent();

QString errorMessageFromErrorCode(ErrorCodeType errorCode);
}
}

#endif // MUSEC_NATIVE_NATIVE
