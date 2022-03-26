#include "ASIOCallback.hpp"

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
    // 以下代码可能在 ASIO 驱动被卸载后执行，因此需要额外的检验。
    // 加锁表面上更好，但此函数可能在中断时刻调用，因此上锁可能会影响效率。
    if(driver)
    {
        if(driver->getSamplePosition(&(timeInfo.timeInfo.samplePosition),
                                     &(timeInfo.timeInfo.systemTime))
           == ASE_OK)
        {
            timeInfo.timeInfo.flags = AsioTimeInfoFlags::kSystemTimeValid
                                    | AsioTimeInfoFlags::kSamplePositionValid;
        }
        onASIOBufferSwitchTimeInfo(&timeInfo, doubleBufferIndex, directProcess);
    }
}

ASIOTime* onASIOBufferSwitchTimeInfo(ASIOTime* params,
                                     long doubleBufferIndex,
                                     ASIOBool directProcess)
{
    auto& bufferInfoList = getASIOBufferInfoList();
    auto& channelInfoList = getASIOChannelInfoList();
    for(int i = 0; i < bufferInfoList.size(); ++i)
    {
        if(bufferInfoList[i].isInput)
        {
            switch(channelInfoList[i].type)
            {
                case ASIOSTInt16MSB:
                    break;
                case ASIOSTInt24MSB:
                    break;
                case ASIOSTInt32MSB:
                    break;
                case ASIOSTFloat32MSB:
                    break;
                case ASIOSTFloat64MSB:
                    break;
                case ASIOSTInt32MSB16:
                    break;
                case ASIOSTInt32MSB18:
                    break;
                case ASIOSTInt32MSB20:
                    break;
                case ASIOSTInt32MSB24:
                    break;
                case ASIOSTInt16LSB:
                    break;
                case ASIOSTInt24LSB:
                    break;
                case ASIOSTInt32LSB:
                    break;
                case ASIOSTFloat32LSB:
                    break;
                case ASIOSTFloat64LSB:
                    break;
                case ASIOSTInt32LSB16:
                    break;
                case ASIOSTInt32LSB18:
                    break;
                case ASIOSTInt32LSB20:
                    break;
                case ASIOSTInt32LSB24:
                    break;
                case ASIOSTDSDInt8LSB1:
                    break;
                case ASIOSTDSDInt8MSB1:
                    break;
                case ASIOSTDSDInt8NER8:
                    break;
                case ASIOSTLastEntry:
                    break;
                default:
                    break;
            }
        }
        else
        {
            switch(channelInfoList[i].type)
            {
                case ASIOSTInt16MSB:
                    break;
                case ASIOSTInt24MSB:
                    break;
                case ASIOSTInt32MSB:
                    break;
                case ASIOSTFloat32MSB:
                    break;
                case ASIOSTFloat64MSB:
                    break;
                case ASIOSTInt32MSB16:
                    break;
                case ASIOSTInt32MSB18:
                    break;
                case ASIOSTInt32MSB20:
                    break;
                case ASIOSTInt32MSB24:
                    break;
                case ASIOSTInt16LSB:
                    break;
                case ASIOSTInt24LSB:
                    break;
                case ASIOSTInt32LSB:
                    break;
                case ASIOSTFloat32LSB:
                    break;
                case ASIOSTFloat64LSB:
                    break;
                case ASIOSTInt32LSB16:
                    break;
                case ASIOSTInt32LSB18:
                    break;
                case ASIOSTInt32LSB20:
                    break;
                case ASIOSTInt32LSB24:
                    break;
                case ASIOSTDSDInt8LSB1:
                    break;
                case ASIOSTDSDInt8MSB1:
                    break;
                case ASIOSTDSDInt8NER8:
                    break;
                case ASIOSTLastEntry:
                    break;
                default:
                    break;
            }
        }
    }
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
        case kAsioResetRequest:
        {
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
