#ifndef MUSEC_AUDIO_DRIVER_ASIODRIVER
#define MUSEC_AUDIO_DRIVER_ASIODRIVER

#include "audio/driver/ASIODriverForward.hpp"
#include "audio/driver/ASIODriverBasicInfo.hpp"
#include "audio/driver/ASIODriverStreamInfo.hpp"

#include <QString>
#include <QList>

#include <memory>

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
using ASIOBufferInfoList = std::array<ASIOBufferInfo, channelCount>;
using ASIOChannelInfoList = std::array<ASIOChannelInfo, channelCount>;

ASIOBufferInfoList& getASIOBufferInfoList();

ASIOChannelInfoList& getASIOChannelInfoList();

enum ASIODriverField
{
    NameField = 0,
    CLSIDField
};

QList<ASIODriverBasicInfo> enumerateDrivers();

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
public:
    void swap(ASIODriver& rhs);
private:
    ASIODriverBasicInfo driverInfo_;
    IASIO* driver_; // Is shared_ptr needed?
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

ASIODriverStreamInfo getASIODriverStreamInfo(const ASIODriver& driver = AppASIODriver());
}
}
}

namespace std
{
template<>
void swap(Musec::Audio::Driver::ASIODriver& lhs, Musec::Audio::Driver::ASIODriver& rhs)
    noexcept(std::is_move_assignable_v<Musec::Audio::Driver::ASIODriver>);
}

#endif // MUSEC_AUDIO_DRIVER_ASIODRIVER
