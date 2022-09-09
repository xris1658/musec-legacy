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

IASIO* ASIODriverImpl::driver() const
{
    return driver_;
}

ASIODriverImpl::operator IASIO*() const
{
    return driver();
}

IASIO* ASIODriverImpl::operator*() const
{
    return driver();
}

IASIO* ASIODriverImpl::operator->() const
{
    return driver();
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
