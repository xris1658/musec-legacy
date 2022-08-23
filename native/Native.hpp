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
constexpr auto CLSIDStringLength = 38;

using SystemTimeType = SYSTEMTIME;

using ThreadMaskType = std::uint64_t;

using SystemTimeStringType = std::array<char, 18>;

using WindowType = HWND;

using ErrorCodeType = decltype(GetLastError());
// Usually <User folder>\AppData\Roaming
const QString& roamingAppDataFolder();

// Usually <System Driver>\Program Files
const QString& programFilesFolder();

// Usually <User folder>\AppData\Local
const QString& localAppDataFolder();

// Path to Musec data, usually <User folder>\AppData\Roaming\Musec
const QString& dataDirectoryPath();

SystemTimeType getCurrentTime();

SystemTimeType getLaunchTime();

// Format systen time to YYYYMMDDHHMMSS
SystemTimeStringType formatTime(const SystemTimeType& time);

void openSpecialCharacterInput();

void showFileInExplorer(const QString& path);

int getProcessCPUCoreCount();

ThreadMaskType getMIDIClockThreadAffinity();

ThreadMaskType setThreadMask(ThreadMaskType mask);

std::int64_t currentTimeInNanosecond();

void setThreadPriorityToTimeCritical();

inline constexpr Musec::Util::Endian endian()
{
    return Q_BYTE_ORDER == Q_LITTLE_ENDIAN? Musec::Util::Endian::LittleEndian:
        Q_BYTE_ORDER == Q_BIG_ENDIAN? Musec::Util::Endian::BigEndian:
            Musec::Util::Endian::UnknownEndian;
}

// Check if the application is being debugged. Currently only used to decide whether system
// rendering is enabled.
bool isDebuggerPresent();

QString errorMessageFromErrorCode(ErrorCodeType errorCode);

QString getProductVersion(const QString& path);

QList<Musec::Audio::Driver::ASIODriverBasicInfo> enumerateDrivers();
}
}

#endif // MUSEC_NATIVE_NATIVE
