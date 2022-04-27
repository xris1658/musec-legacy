#include "Native.hpp"

#include "base/Base.hpp"
#include "base/Constants.hpp"

#include <QtGlobal>

#include <ShlObj_core.h>
#include <shellapi.h>
#include <processthreadsapi.h>
#include <timezoneapi.h>
#include <realtimeapiset.h>

#include <array>
#include <bitset>
#include <charconv>
#include <chrono>
#include <cstring>
#include <stdexcept>

namespace Musec
{
namespace Native
{
namespace Impl
{
std::uint64_t procMask()
{
    std::uint64_t procMask = 0;
    std::uint64_t sysMask = 0;
    if(GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask))
    {
        return procMask;
    }
    return 0;
}

std::int64_t getQPF()
{
    LARGE_INTEGER ret;
    QueryPerformanceFrequency(&ret);
    return ret.QuadPart;
}

std::int64_t qpf()
{
    static auto ret = getQPF();
    return ret;
}
}
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

void openSpecialCharacterInput()
{
    ShellExecuteA(nullptr, "open", "charmap.exe", nullptr, nullptr, SW_NORMAL);
}

void showFileInExplorer(const QString& path)
{
    constexpr char part1Raw[] = "explorer /select,";
    constexpr auto size = Musec::Base::stackArraySize(part1Raw) - 1;
    QString command = part1Raw;
    for(int i = 0; i < path.size(); ++i)
    {
        command[int(i + size)] = path[i] == '/'? '\\': path[i];
    }
    auto commandWide = command.toStdWString();
    ShellExecuteW(nullptr, L"open", commandWide.data(), nullptr, nullptr, SW_NORMAL);
}

int getProcessCPUCoreCount()
{
    auto procMask = Impl::procMask();
    auto bitset = reinterpret_cast<std::bitset<sizeof(procMask) * CHAR_BIT>*>(&procMask);
    return bitset->count();
}

ThreadMaskType getMIDIClockThreadAffinity()
{
    static auto procMask = Impl::procMask();
    if(procMask == 0)
    {
        return 1;
    }
    std::uint64_t ret = 1;
    while((ret & procMask) == 0)
    {
        ret <<= 1;
    }
    return ret;
}

ThreadMaskType setThreadMask(ThreadMaskType mask)
{
    auto ret = SetThreadAffinityMask(GetCurrentThread(), mask);
    if(ret == 0)
    {
        throw std::runtime_error("");
    }
    return ret;
}

std::int64_t currentTimeInNanosecond()
{
    // // Ver 1（Windows API）
    // auto qpf = Impl::qpf();
    // LARGE_INTEGER qpc;
    // QueryPerformanceCounter(&qpc);
    // return qpc.QuadPart * 1e9 / static_cast<double>(qpf);
    // Ver 1（现代 C++ API）
    // std::chrono::steady_clock 在 Windows，MSVC 平台使用 QPC 实现。
    // https://github.com/microsoft/STL/blob/main/stl/inc/__msvc_chrono.hpp#L668
    // 要保证获取时间的连贯性，用户需要保证此函数所在的线程只会在一个 CPU 核心上运行。
    return std::chrono::steady_clock::now().time_since_epoch().count();

     // // Ver 2
     // std::uint64_t ret;
     // QueryInterruptTimePrecise(&ret);
     // return ret * 100;
}

void setThreadPriorityToTimeCritical()
{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
}

constexpr Musec::Util::Endian endian()
{
    return Q_BYTE_ORDER == Q_LITTLE_ENDIAN? Musec::Util::Endian::LittleEndian:
        Q_BYTE_ORDER == Q_BIG_ENDIAN? Musec::Util::Endian::BigEndian:
        Musec::Util::Endian::UnknownEndian;
}

bool isDebuggerPresent()
{
    return IsDebuggerPresent();
}

}
}
