#ifndef MUSEC_NATIVE_NATIVE
#define MUSEC_NATIVE_NATIVE

#include "audio/driver/ASIODriverBasicInfo.hpp"
#include "audio/driver/ASIODriverForward.hpp"
#include "util/Endian.hpp"

#include <QList>
#include <QString>
#include <QtGlobal>

#include <Windows.h>

#include <array>

namespace Musec
{
namespace Native
{
// CLSID 字符串的长度（32 位数据 + 4 个横杠 + 2 个花括号，不包含 NULL 终止符）
constexpr auto CLSIDStringLength = 38;

using SystemTimeType = SYSTEMTIME;

using ThreadMaskType = std::uint64_t;

using SystemTimeStringType = std::array<char, 18>;

using WindowType = HWND;

using ErrorCodeType = decltype(GetLastError());
// 漫游用户数据路径（用户文件夹\AppData\Roaming）
const QString& roamingAppDataFolder();

// 程序安装目录（系统盘\Program Files）
const QString& programFilesFolder();

// 本地用户数据路径（用户文件夹\AppData\Local）
const QString& localAppDataFolder();

// Musec 程序用户数据路径（用户文件夹\AppData\Roaming\Musec）
const QString& dataDirectoryPath();

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

inline constexpr Musec::Util::Endian endian()
{
    return Q_BYTE_ORDER == Q_LITTLE_ENDIAN? Musec::Util::Endian::LittleEndian:
        Q_BYTE_ORDER == Q_BIG_ENDIAN? Musec::Util::Endian::BigEndian:
            Musec::Util::Endian::UnknownEndian;
}

// 检测应用自身是否处于调试状态。目前仅用于辅助选择文字渲染。
bool isDebuggerPresent();

QString errorMessageFromErrorCode(ErrorCodeType errorCode);

QString getProductVersion(const QString& path);

QList<Musec::Audio::Driver::ASIODriverBasicInfo> enumerateDrivers();
}
}

#endif // MUSEC_NATIVE_NATIVE
