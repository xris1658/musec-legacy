#include "ASIOCallback.hpp"

#include "controller/ASIODriverController.hpp"
#include "audio/driver/ASIODriver.hpp"
#include "base/Base.hpp"
#include "controller/ASIODriverController.hpp"

namespace Musec::Audio::Driver
{
void onASIOBufferSwitch(long doubleBufferIndex, ASIOBool directProcess)
{
    ASIOTime timeInfo;
    Musec::Base::memoryReset(&timeInfo);
    auto& driver = AppASIODriver();
    if(driver->getSamplePosition(&(timeInfo.timeInfo.samplePosition),
                                 &(timeInfo.timeInfo.systemTime))
       == ASE_OK)
    {
        timeInfo.timeInfo.flags = AsioTimeInfoFlags::kSystemTimeValid
                                | AsioTimeInfoFlags::kSamplePositionValid;
    }
    onASIOBufferSwitchTimeInfo(&timeInfo, doubleBufferIndex, directProcess);
}

ASIOTime* onASIOBufferSwitchTimeInfo(ASIOTime* params,
                                     long doubleBufferIndex,
                                     ASIOBool directProcess)
{
    // TODO
    return nullptr;
}

void onASIOSampleRateDidChange(ASIOSampleRate sRate)
{
    // TODO
}

long onASIOMessage(long selector,
                   long value,
                   void* message,
                   double* opt)
{
    // TODO
    long ret = 0;
    switch(selector)
    {
        case kAsioSelectorSupported:
            if(value == kAsioResetRequest
            || value == kAsioEngineVersion
            || value == kAsioResyncRequest
            || value == kAsioLatenciesChanged
            || value == kAsioSupportsTimeInfo
            || value == kAsioSupportsTimeCode
            || value == kAsioSupportsInputMonitor)
                ret = 1L;
            break;
        // 重置 ASIO 驱动
        case kAsioResetRequest:
        {
            auto driverInfo = AppASIODriver().driverInfo();
            AppASIODriver() = ASIODriver();
            AppASIODriver() = ASIODriver(driverInfo);
            Controller::ASIODriverController::loadASIODriver();
            //
            ret = 1L;
            break;
        }
        case kAsioResyncRequest:
            ret = 1L;
            break;

        case kAsioLatenciesChanged:
            ret = 1L;
            break;

        case kAsioEngineVersion:
            ret = 2L;
            break;

        case kAsioSupportsTimeInfo:
            ret = 1;
            break;

        case kAsioSupportsTimeCode:
            ret = 0;
            break;
        default:
            ret = 0;
            break;
    }
    return ret;
}

ASIOCallbacks& getCallbacks()
{
    static ASIOCallbacks ret
    {
        &onASIOBufferSwitch,
        &onASIOSampleRateDidChange,
        &onASIOMessage,
        &onASIOBufferSwitchTimeInfo
    };
    return ret;
}
}
