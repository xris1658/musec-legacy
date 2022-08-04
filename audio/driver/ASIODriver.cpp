#include "ASIODriver.hpp"

#include "native/Native.hpp"

#include <Windows.h>
#include <winreg.h>
#include <winerror.h>

#include <array>
#include <stdexcept>
#include <tuple>
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

ASIODriver::ASIODriver(): driverInfo_(std::tuple<QString, QString>("", "")), driver_(nullptr) {}

ASIODriver::ASIODriver(const ASIODriverBasicInfo& info): driverInfo_(info), driver_(nullptr)
{
    CoInitialize(NULL);
    wchar_t clsidBuffer[Musec::Native::CLSIDStringLength + 1];
    const auto& string = std::get<ASIODriverField::CLSIDField>(info);
    clsidBuffer[string.size()] = L'\0';
    string.toWCharArray(clsidBuffer);
    CLSID clsid;
    auto convertToCLSIDResult = CLSIDFromString(clsidBuffer, &clsid);
    if(convertToCLSIDResult != NOERROR)
    {
        throw std::runtime_error("Load driver info failed.");
    }
    auto loadDriverResult = CoCreateInstance(clsid, nullptr,
        CLSCTX_INPROC_SERVER, clsid,
        reinterpret_cast<void**>(&driver_)
    );
    if(loadDriverResult != S_OK)
    {
        throw std::runtime_error("Load driver failed.");
    }
}

ASIODriver::ASIODriver(ASIODriver&& rhs) noexcept: driverInfo_(std::tuple<QString, QString>("", "")), driver_(nullptr)
{
    swap(rhs);
}

ASIODriver& ASIODriver::operator=(ASIODriver&& rhs) noexcept
{
    swap(rhs);
    return *this;
}

ASIODriver::~ASIODriver()
{
    if(driver_)
    {
        driver_->stop();
        driver_->disposeBuffers();
        driver_->Release();
        driver_ = nullptr;
        CoUninitialize();
    }
}

ASIODriverBasicInfo ASIODriver::driverInfo() const
{
    return driverInfo_;
}

IASIO* ASIODriver::driver() const
{
    return driver_;
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

void ASIODriver::swap(ASIODriver& rhs)
{
    std::swap(driver_, rhs.driver_);
    std::swap(driverInfo_, rhs.driverInfo_);
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

namespace std
{
template<> void swap(Musec::Audio::Driver::ASIODriver& lhs, Musec::Audio::Driver::ASIODriver& rhs)
noexcept(std::is_move_constructible_v<Musec::Audio::Driver::ASIODriver>
         && std::is_move_assignable_v<Musec::Audio::Driver::ASIODriver>)
{
    lhs.swap(rhs);
}
}
