#include "ASIODriver.hpp"

#include "native/ASIODriverImpl.hpp"

#include <array>
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

QList<ASIODriverBasicInfo> enumerateASIODriverInfo()
{
    return Musec::Native::enumerateASIODriverInfo();
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
    return pImpl_? pImpl_->driver(): nullptr;
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
    ASIOChannelCount ret {0, 0};
    if(driver)
    {
        driver->getChannels(&ret.inputCount, &ret.outputCount);
    }
    return ret;
}

ASIOLatencyInSamples getLatency(const ASIODriver& driver)
{
    ASIOLatencyInSamples ret {0, 0};
    if(driver)
    {
        driver->getLatencies(&ret.inputLatency, &ret.outputLatency);
    }
    return ret;
}

ASIOBufferSize getBufferSize(const ASIODriver& driver)
{
    ASIOBufferSize ret {0, 0, 0, 0};
    if(driver)
    {
        driver->getBufferSize(&ret.minimumBufferSize,
            &ret.maximumBufferSize,
            &ret.preferredBufferSize,
            &ret.bufferSizeGranularity);
    }
    return ret;
}

ASIOSampleRate getSampleRate(const ASIODriver& driver)
{
    ASIOSampleRate ret = 0;
    if(driver)
    {
        driver->getSampleRate(&ret);
    }
    return ret;
}

}
