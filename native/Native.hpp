#ifndef MUSEC_NATIVE_NATIVE
#define MUSEC_NATIVE_NATIVE

#include <QString>

#include <Windows.h>

#include <array>

namespace Musec
{
namespace Native
{
using SystemTimeType = SYSTEMTIME;

using SystemTimeStringType = std::array<char, 18>;
// 漫游用户数据路径（用户文件夹\AppData\Roaming）
const QString& RoamingDirectoryPath();

// Musec 程序用户数据路径（用户文件夹\AppData\Roaming\Musec）
const QString& DataDirectoryPath();

SystemTimeType getCurrentTime();

SystemTimeType getLaunchTime();

// 获取系统时间类型，并转换为YYYYMMDDHHMMSS 格式的字符串
SystemTimeStringType formatTime(const SystemTimeType& time);
}
}

#endif // MUSEC_NATIVE_NATIVE
