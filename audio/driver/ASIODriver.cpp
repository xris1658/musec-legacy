#include "ASIODriver.hpp"

#include <Windows.h>
#include <winreg.h>
#include <winerror.h>

#include <array>
#include <tuple>
#include <utility>

namespace Musec::Audio::Driver
{
ASIOBufferInfoList& getASIOBufferInfoList()
{
    static ASIOBufferInfoList ret;
    return ret;
}

QList<ASIODriverBasicInfo> enumerateDrivers()
{
    QList<ASIODriverBasicInfo> ret;
    constexpr auto driverNameSize = 256;
    std::array<wchar_t, driverNameSize> buffer = {0};
    HKEY hKey;
    auto findKeyResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
        L"SOFTWARE\\ASIO", 0, KEY_READ | KEY_WOW64_64KEY, &hKey);
    if(findKeyResult != ERROR_SUCCESS)
    {
        // 异常处理
    }
    DWORD numSubKey;
    auto queryInfoKeyResult = RegQueryInfoKeyW(hKey, NULL, NULL, NULL,
        &numSubKey, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    if(queryInfoKeyResult != ERROR_SUCCESS)
    {
        // 异常处理
    }
    if(numSubKey > 0)
    {
        for(decltype(numSubKey) i = 0; i < numSubKey; ++i)
        {
            DWORD driverNameLength = driverNameSize;
            HKEY subKey;
            auto enumKeyExResult = RegEnumKeyExW(hKey, i,
                buffer.data(), &driverNameLength, NULL, NULL, NULL, NULL);
            if(enumKeyExResult != ERROR_SUCCESS
            && enumKeyExResult != ERROR_MORE_DATA)
            {
                // 异常处理
            }
            else
            {
                // 打开项，读取名称和 ID
                auto openKeyResult = RegOpenKeyExW(hKey, buffer.data(), 0,
                    KEY_READ | KEY_WOW64_64KEY | KEY_QUERY_VALUE, &subKey);
                if(openKeyResult != ERROR_SUCCESS)
                {
                    // 异常处理
                }
                else
                {
                    const wchar_t name[] = L"CLSID";
                    std::array<wchar_t, CLSIDStringLength + 1> clsidBuffer = {0};
                    DWORD clsidBufferLength;
                    auto getValueResult = RegGetValueW(subKey, NULL, name,
                        RRF_RT_REG_SZ, NULL, clsidBuffer.data(), &clsidBufferLength);
                    if(getValueResult != ERROR_SUCCESS)
                    {
                        // 异常处理
                    }
                    else
                    {
                        ret.append(
                            std::make_tuple(
                                QString::fromWCharArray(buffer.data()),
                                QString::fromWCharArray(clsidBuffer.data())
                            )
                        );
                    }
                }
            }
        }
    }
    return ret;
}

ASIODriver::ASIODriver(): driverInfo_(std::tuple<QString, QString>("", "")), driver_(nullptr) {}

ASIODriver::ASIODriver(const ASIODriverBasicInfo& info): driverInfo_(info), driver_(nullptr)
{
    CoInitialize(NULL);
    std::array<wchar_t, CLSIDStringLength + 1> clsidBuffer = {0};
    std::get<ASIODriverField::CLSIDField>(info).toWCharArray(clsidBuffer.data());
    CLSID clsid;
    auto convertToCLSIDResult = CLSIDFromString(clsidBuffer.data(), &clsid);
    if(convertToCLSIDResult != NOERROR)
    {
        // 异常处理
    }
    auto loadDriverResult = CoCreateInstance(clsid, nullptr,
        CLSCTX_INPROC_SERVER, clsid,
        reinterpret_cast<void**>(&driver_)
    );
    if(loadDriverResult != S_OK)
    {
        // 异常处理
    }
}

ASIODriver::ASIODriver(ASIODriver&& rhs) noexcept: driverInfo_(std::tuple<QString, QString>("", "")), driver_(nullptr)
{
    std::swap(driverInfo_, rhs.driverInfo_);
    std::swap(driver_, rhs.driver_);
}

ASIODriver& ASIODriver::operator=(ASIODriver&& rhs) noexcept
{
    std::swap(driverInfo_, rhs.driverInfo_);
    std::swap(driver_, rhs.driver_);
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

// QList<ASIODriverBasicInfo> ASIODriver::enumerateDrivers()
// {
//     return Musec::Audio::Driver::enumerateDrivers();
// }

ASIODriver& AppASIODriver()
{
    static Musec::Audio::Driver::ASIODriver ret;
    return ret;
}

ASIODriverStreamInfo getASIODriverStreamInfo(const ASIODriver& driver)
{
    std::array<char, 64> name = {0};
    driver->getDriverName(name.data());
    // auto version = driver->getDriverVersion();
    ASIODriverStreamInfo ret;
    driver->getChannels(&ret.inputChannelCount,
                        &ret.outputChannelCount);
    driver->getLatencies(&ret.inputLatencyInSamples,
                         &ret.outputLatencyInSamples);
    driver->getBufferSize(&ret.minimumBufferSize,
                          &ret.maximumBufferSize,
                          &ret.preferredBufferSize,
                          &ret.bufferSizeGranularity);
    driver->getSampleRate(&ret.sampleRate);
    return ret;
}
}
