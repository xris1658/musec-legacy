#include "ASIODriver.hpp"

#include "audio/driver/ASIOCallback.hpp"
#include "native/ASIODriverImpl.hpp"

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

void prepareBufferInfo(const ASIODriver& driver)
{
    auto& bufferInfoList = getASIOBufferInfoList();
    if(driver)
    {
        auto [inputCount, outputCount] = getChannelCount(driver);
        bufferInfoList.resize(inputCount + outputCount, {});
        for(int i = 0; i < inputCount; ++i)
        {
            bufferInfoList[i].isInput = true;
            bufferInfoList[i].channelNum = i;
            bufferInfoList[i].buffers[0] = bufferInfoList[i].buffers[1] = nullptr;
        }
        for(int i = 0; i < outputCount; ++i)
        {
            bufferInfoList[i + inputCount].isInput = false;
            bufferInfoList[i + inputCount].channelNum = i;
            bufferInfoList[i + inputCount].buffers[0] = bufferInfoList[i + inputCount].buffers[1]
                = nullptr;
        }
    }
    else
    {
        bufferInfoList.clear();
    }
}

void prepareChannelInfo(const ASIODriver& driver)
{
    auto& channelInfoList = getASIOChannelInfoList();
    if(driver)
    {
        auto [inputCount, outputCount] = getChannelCount(driver);
        channelInfoList.resize(inputCount + outputCount, {});
        for(int i = 0; i < inputCount; ++i)
        {
            channelInfoList[i].isInput = true;
            channelInfoList[i].channel = i;
        }
        for(int i = 0; i < outputCount; ++i)
        {
            channelInfoList[i + inputCount].isInput = false;
            channelInfoList[i + inputCount].channel = i;
        }
    }
    else
    {
        channelInfoList.clear();
    }
}
}
