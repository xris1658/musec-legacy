#ifndef MUSEC_AUDIO_DRIVER_ASIODRIVER
#define MUSEC_AUDIO_DRIVER_ASIODRIVER

#include "audio/driver/ASIODriverForward.hpp"
#include "audio/driver/ASIODriverBasicInfo.hpp"

#include <QString>
#include <QList>

#include <memory>
#include <vector>

namespace Musec
{
namespace Native
{
// Forward declaration
class ASIODriverImpl;
}
}

namespace Musec
{
namespace Audio
{
namespace Driver
{
extern double cpuUsage;

constexpr int maxInputChannelCount = 16;
constexpr int maxOutputChannelCount = 16;
constexpr int channelCount = maxInputChannelCount + maxOutputChannelCount;
using ASIOBufferInfoList = std::vector<ASIOBufferInfo>;
using ASIOChannelInfoList = std::vector<ASIOChannelInfo>;

ASIOBufferInfoList& getASIOBufferInfoList();

ASIOChannelInfoList& getASIOChannelInfoList();

enum ASIODriverField
{
    NameField = 0,
    CLSIDField
};

std::vector<ASIODriverBasicInfo> enumerateASIODriverInfo();

struct ASIOChannelCount
{
    long inputCount;
    long outputCount;
};

struct ASIOLatencyInSamples
{
    long inputLatency;
    long outputLatency;
};

struct ASIOBufferSize
{
    long minimumBufferSize;
    long maximumBufferSize;
    long preferredBufferSize;
    long bufferSizeGranularity;
};

class ASIODriver
{
public:
    ASIODriver();
    explicit ASIODriver(const ASIODriverBasicInfo& info);
    ASIODriver(const ASIODriver&) = delete;
    ASIODriver& operator=(const ASIODriver&) = delete;
    ASIODriver(ASIODriver&&) noexcept;
    ASIODriver& operator=(ASIODriver&&) noexcept;
    ~ASIODriver();
public:
    ASIODriverBasicInfo driverInfo() const;
    inline IASIO* driver() const;
    operator IASIO*() const;
    IASIO* operator*() const;
    IASIO* operator->() const;
private:
    std::unique_ptr<Musec::Native::ASIODriverImpl> pImpl_;
};

// Returns the ASIO driver of the application.
// Remarks:
// The initialization of `IASIO` needs the handle of a window.
// Musec uses the main window, see `Musec::Event::EventHandler::onMainWindowOpened()`).
// Therefore we need to uninitialize `IASIO` before closing the main window.
// If we try destroying the main window before uninitializing `IASIO`, then the uninitialization
// will encounter problems. This might make the application unable to exit, or crash the application.
ASIODriver& AppASIODriver();

ASIOChannelCount getChannelCount(const ASIODriver& driver = AppASIODriver());

ASIOLatencyInSamples getLatency(const ASIODriver& driver = AppASIODriver());

ASIOBufferSize getBufferSize(const ASIODriver& driver = AppASIODriver());

ASIOSampleRate getSampleRate(const ASIODriver& driver = AppASIODriver());

// Resize getASIOBufferInfoList() and fill input info.
// e.g. For a driver that has 2 inputs and 4 outputs, the buffer info list will be
// | Field      | 0 | 1 | 2 | 3 | 4 | 5 |
// | ---------- | - | - | - | - | - | - |
// | isInput    | 1 | 1 | 0 | 0 | 0 | 0 |
// | channelNum | 0 | 1 | 0 | 1 | 2 | 3 |
void prepareBufferInfo(const ASIODriver& driver = AppASIODriver());

// Resize getASIOChannelInfoList() and fill input info.
// e.g. For a driver that has 2 inputs and 4 outputs, the channel info list will be
// | Field   | 0 | 1 | 2 | 3 | 4 | 5 |
// | ------- | - | - | - | - | - | - |
// | channel | 0 | 1 | 0 | 1 | 2 | 3 |
// | isInput | 1 | 1 | 0 | 0 | 0 | 0 |
void prepareChannelInfo(const ASIODriver& driver = AppASIODriver());

void fillOutput1WithZero(long bufferSize);
}
}
}

#endif // MUSEC_AUDIO_DRIVER_ASIODRIVER
