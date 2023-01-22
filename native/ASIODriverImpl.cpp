#include "ASIODriverImpl.hpp"

#include "audio/driver/ASIODriver.hpp"
#include "native/Native.hpp"

#include <Windows.h>
#include <winreg.h>
#include <winerror.h>

#include <stdexcept>
#include <tuple>
#include <utility>

namespace Musec::Native
{
using namespace Musec::Audio::Driver;

std::vector<Musec::Audio::Driver::ASIODriverBasicInfo> enumerateASIODriverInfo()
{
    std::vector<Musec::Audio::Driver::ASIODriverBasicInfo> ret;
    constexpr auto driverNameSize = 256;
    std::array<wchar_t, driverNameSize> buffer = {0};
    HKEY hKey;
    auto findKeyResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
        L"SOFTWARE\\ASIO", 0, KEY_READ | KEY_WOW64_64KEY, &hKey);
    if(findKeyResult != ERROR_SUCCESS)
    {
        throw std::runtime_error("Can't enumerate ASIO drivers.");
    }
    DWORD numSubKey;
    auto queryInfoKeyResult = RegQueryInfoKeyW(hKey, NULL, NULL, NULL,
        &numSubKey, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    if(queryInfoKeyResult != ERROR_SUCCESS)
    {
        throw std::runtime_error("Can't enumerate ASIO drivers.");
    }
    if(numSubKey > 0)
    {
        ret.reserve(numSubKey);
        for(decltype(numSubKey) i = 0; i < numSubKey; ++i)
        {
            DWORD driverNameLength = driverNameSize;
            HKEY subKey;
            auto enumKeyExResult = RegEnumKeyExW(hKey, i,
                buffer.data(), &driverNameLength, NULL, NULL, NULL, NULL);
            if(enumKeyExResult != ERROR_SUCCESS
               && enumKeyExResult != ERROR_MORE_DATA)
            {
                throw std::runtime_error("A error occured while enumerating ASIO drivers.");
            }
            else
            {
                auto openKeyResult = RegOpenKeyExW(hKey, buffer.data(), 0,
                    KEY_READ | KEY_WOW64_64KEY | KEY_QUERY_VALUE, &subKey);
                if(openKeyResult != ERROR_SUCCESS)
                {
                    throw std::runtime_error("A error occured while enumerating ASIO drivers.");
                }
                else
                {
                    const wchar_t name[] = L"CLSID";
                    std::array<wchar_t, CLSIDStringLength + 1> clsidBuffer = {0};
                    DWORD clsidBufferLength;
                    auto getValueResult = RegGetValueW(subKey, NULL, name,
                        RRF_RT_REG_SZ, NULL, clsidBuffer.data(), &clsidBufferLength);
                    if(getValueResult == ERROR_SUCCESS && clsidBufferLength == clsidBuffer.size() * sizeof(wchar_t))
                    {
                        ret.emplace_back(
                            std::make_tuple(
                                QString::fromWCharArray(buffer.data()),
                                QString::fromWCharArray(clsidBuffer.data())
                            )
                        );
                    }
                    else
                    {
                        // The information is invalid. Need to inform the user?
                    }
                }
            }
        }
    }
    return ret;
}

ASIODriverImpl::ASIODriverImpl(): driverInfo_(std::tuple<QString, QString>("", "")), driver_(nullptr) {}

ASIODriverImpl::ASIODriverImpl(const ASIODriverBasicInfo& info): driverInfo_(info), driver_(nullptr)
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

ASIODriverImpl::ASIODriverImpl(ASIODriverImpl&& rhs) noexcept: driverInfo_(std::tuple<QString, QString>("", "")), driver_(nullptr)
{
    swap(rhs);
}

ASIODriverImpl& ASIODriverImpl::operator=(ASIODriverImpl&& rhs) noexcept
{
    swap(rhs);
    return *this;
}

ASIODriverImpl::~ASIODriverImpl()
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

ASIODriverBasicInfo ASIODriverImpl::driverInfo() const
{
    return driverInfo_;
}

void ASIODriverImpl::swap(ASIODriverImpl& rhs)
{
    std::swap(driver_, rhs.driver_);
    std::swap(driverInfo_, rhs.driverInfo_);
}
}

namespace std
{
template<> void swap(Musec::Native::ASIODriverImpl& lhs, Musec::Native::ASIODriverImpl& rhs)
noexcept(std::is_move_assignable_v<Musec::Native::ASIODriverImpl>)
{
    lhs.swap(rhs);
}
}
