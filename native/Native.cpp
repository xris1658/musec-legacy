#include "Native.hpp"

#include "base/Constants.hpp"

#include <ShlObj_core.h>
#include <processthreadsapi.h>
#include <timezoneapi.h>

#include <array>
#include <charconv>
#include <cstring>

namespace Musec
{
namespace Native
{
const QString& RoamingDirectoryPath()
{
    static wchar_t path[MAX_PATH] = {0};
    auto getFolderResult = SHGetFolderPathW(nullptr, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, path);
    if(getFolderResult != S_OK)
    {
        // TODO: 异常处理
    }
    static auto ret = QString::fromWCharArray(path);
    return ret;
}

const QString& DataDirectoryPath()
{
    static QString ret = QString(RoamingDirectoryPath()).append("\\").append(Musec::Base::ProductName);
    return ret;
}

SystemTimeType getCurrentTime()
{
    SystemTimeType ret;
    GetSystemTime(&ret);
    return ret;
}

SystemTimeType getLaunchTime()
{
    auto process = GetCurrentProcess();
    FILETIME processTime[4];
    auto getTime = GetProcessTimes(process, processTime, processTime + 1, processTime + 2, processTime + 3);
    if(!getTime)
    {
        // 异常处理
    }
    SYSTEMTIME ret;
    if(!FileTimeToSystemTime(processTime, &ret))
    {
        // 异常处理
    }
    return ret;
}

SystemTimeStringType formatTime(const SystemTimeType& time)
{
    std::array<char, 18> ret = {0};
    std::to_chars(ret.data(), ret.data() + 4, time.wYear);
    std::to_chars(ret.data() + 4, ret.data() + 6, time.wMonth);
    if (ret[5] == 0)
    {
        ret[5] = ret[4];
        ret[4] = '0';
    }
    std::to_chars(ret.data() + 6, ret.data() + 8, time.wDay);
    if (ret[7] == 0)
    {
        ret[7] = ret[6];
        ret[6] = '0';
    }
    std::to_chars(ret.data() + 8, ret.data() + 10, time.wHour);
    if (ret[9] == 0 && ret[8])
    {
        ret[9] = ret[8];
        ret[8] = '0';
    }
    std::to_chars(ret.data() + 10, ret.data() + 12, time.wMinute);
    if (ret[11] == 0 && ret[10])
    {
        ret[11] = ret[10];
        ret[10] = '0';
    }
    std::to_chars(ret.data() + 12, ret.data() + 14, time.wSecond);
    if (ret[13] == 0 && ret[12])
    {
        ret[13] = ret[12];
        ret[12] = '0';
    }
    std::to_chars(ret.data() + 14, ret.data() + 17, time.wMilliseconds);
    if(ret[15] == 0)
    {
        ret[16] = ret[15];
        ret[15] = ret[14] = '0';
    }
    else if(ret[16] == 0)
    {
        ret[16] = ret[15];
        ret[15] = ret[14];
        ret[14] = '0';
    }
    return ret;
}
}
}
