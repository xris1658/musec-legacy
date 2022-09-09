#include "ASIODriver.hpp"

#include "native/ASIODriverImpl.hpp"
#include "native/Native.hpp"

#include <utility>

namespace Musec::Audio::Driver
{
ASIOBufferInfoList& getASIOBufferInfoList()
{
    static ASIOBufferInfoList ret;
    return ret;
}

ASIOChannelInfoList& getASIOChannelInfoList()
{
    static ASIOChannelInfoList ret;
    return ret;
}

QList<ASIODriverBasicInfo> enumerateDrivers()
{
    return Musec::Native::enumerateDrivers();
}

ASIODriver::ASIODriver(): pImpl_() {}

ASIODriver::ASIODriver(const ASIODriverBasicInfo& info): pImpl_(std::make_unique<Musec::Native::ASIODriverImpl>(info))
{
}

ASIODriver::ASIODriver(ASIODriver&& rhs) noexcept: pImpl_(std::move(rhs.pImpl_))
{
}

ASIODriver& ASIODriver::operator=(ASIODriver&& rhs) noexcept
{
    pImpl_ = std::move(rhs.pImpl_);
    return *this;
}

ASIODriver::~ASIODriver()
{
}

ASIODriverBasicInfo ASIODriver::driverInfo() const
{
    return pImpl_->driverInfo();
}

IASIO* ASIODriver::driver() const
{
    return pImpl_->driver();
}

ASIODriver::operator IASIO*() const
{
    return driver();
}

IASIO* ASIODriver::operator*() const
{
    return driver();
}

IASIO* ASIODriver::operator->() const
{
    return driver();
}

ASIODriver& AppASIODriver()
{
    static ASIODriver ret;
    return ret;
}

ASIOChannelCount getChannelCount(const ASIODriver& driver)
{
    ASIOChannelCount ret;
    driver->getChannels(&ret.inputCount, &ret.outputCount);
    return ret;
}

ASIOLatencyInSamples getLatency(const ASIODriver& driver)
{
    ASIOLatencyInSamples ret;
    driver->getLatencies(&ret.inputLatency, &ret.outputLatency);
    return ret;
}

ASIOBufferSize getBufferSize(const ASIODriver& driver)
{
    ASIOBufferSize ret;
    driver->getBufferSize(&ret.minimumBufferSize,
        &ret.maximumBufferSize,
        &ret.preferredBufferSize,
        &ret.bufferSizeGranularity);
    return ret;
}

ASIOSampleRate getSampleRate(const ASIODriver& driver)
{
    ASIOSampleRate ret;
    driver->getSampleRate(&ret);
    return ret;
}

ASIODriverStreamInfo getASIODriverStreamInfo(const ASIODriver& driver)
{
    ASIODriverStreamInfo ret {};
    if(*driver)
    {
        // auto version = driver->getDriverVersion();
        driver->getChannels(&ret.inputChannelCount,
                            &ret.outputChannelCount);
        driver->getLatencies(&ret.inputLatencyInSamples,
                             &ret.outputLatencyInSamples);
        driver->getBufferSize(&ret.minimumBufferSize,
                              &ret.maximumBufferSize,
                              &ret.preferredBufferSize,
                              &ret.bufferSizeGranularity);
        driver->getSampleRate(&ret.sampleRate);
    }
    return ret;
}
}
