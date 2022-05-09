#include "ASIOCallback.hpp"

#include "audio/base/Constants.hpp"
#include "audio/driver/ASIODriver.hpp"
#include "base/Base.hpp"
#include "controller/ASIODriverController.hpp"
#include "controller/AudioEngineController.hpp"
#include "native/Native.hpp"
#include "util/Endian.hpp"

namespace Musec::Audio::Driver
{
std::int8_t bufferIndex = 0;
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
    if(*AppASIODriver())
    {
        Musec::Controller::AudioEngineController::AppProject().process();
        std::array<Musec::Audio::Base::AudioBufferView<float>, 2> masterTrackAudioBufferView = {};
        auto bufferSize = getASIODriverStreamInfo(AppASIODriver()).preferredBufferSize;
        masterTrackAudioBufferView[0] = Musec::Audio::Base::AudioBufferView<float>(
            reinterpret_cast<float*>(Musec::Controller::AudioEngineController::AppProject().masterTrackAudioBuffer().data()),
            bufferSize
        );
        masterTrackAudioBufferView[1] = Musec::Audio::Base::AudioBufferView<float>(
            reinterpret_cast<float*>(Musec::Controller::AudioEngineController::AppProject().masterTrackAudioBuffer().data()) + bufferSize,
            bufferSize
        );
        auto currentBufferIndex = bufferIndex;
        if(bufferIndex == 1)
        {
            bufferIndex = 0;
        }
        else
        {
            bufferIndex = 1;
        }
        std::array<int, 64> inputs = {0};
        std::array<int, 64> outputs = {0};
        int inputCount = 0;
        int outputCount = 0;
        auto& bufferInfoList = getASIOBufferInfoList();
        auto& channelInfoList = getASIOChannelInfoList();
        for(int i = 0; i < channelInfoList.size(); ++i)
        {
            if(channelInfoList[i].isActive)
            {
                if(channelInfoList[i].isInput)
                {
                    inputs[inputCount++] = i;
                }
                else
                {
                    outputs[outputCount++] = i;
                }
            }
        }
        for(int i = 0; i < inputCount; ++i)
        {
            auto buffer = bufferInfoList[inputs[i]].buffers;
            // MSB -> BE; LSB -> LE;
            auto type = channelInfoList[i].type;
            switch(type)
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
        for(int i = 0; i < 2; ++i)
        {
            auto buffer = bufferInfoList[outputs[i]].buffers;
            // buffer[0] 和 buffer[1] 是 ASIO 的双缓冲区地址
            // 有些驱动程序 (e.g. ASIO4ALL）不使用双缓冲，两个地址相同
            // 多数驱动程序（FlexASIO）使用双缓冲，两个地址不同
            auto type = channelInfoList[i].type;
            switch(type)
            {
            case ASIOSTInt16MSB:
            {
                std::int16_t* int16Buffer = reinterpret_cast<std::int16_t*>(buffer[currentBufferIndex]);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int16Buffer[j] = static_cast<std::int16_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int16Max);
                    Musec::Util::reverseEndianness(int16Buffer + j, sizeof(int16Buffer[j]));
                }
                break;
            }
            case ASIOSTInt24MSB:
                break;
            case ASIOSTInt32MSB:
            {
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer[currentBufferIndex]);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int32Max);
                    Musec::Util::reverseEndianness(int32Buffer + j, sizeof(int32Buffer[j]));
                }
                break;
            }
            case ASIOSTFloat32MSB:
            {
                float* floatBuffer = reinterpret_cast<float*>(buffer[currentBufferIndex]);
                for(int j = 0; j < bufferSize; ++j)
                {
                    Musec::Util::reverseEndiannessCopy(masterTrackAudioBufferView[i].getSamples() + j, sizeof(float), floatBuffer + j);
                }
            }
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
            {
                std::int16_t* int16Buffer = reinterpret_cast<std::int16_t*>(buffer[currentBufferIndex]);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int16Buffer[j] = static_cast<std::int16_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int16Max);
                    if(Musec::Native::endian() != Musec::Util::Endian::LittleEndian)
                    {
                        Musec::Util::reverseEndianness(masterTrackAudioBufferView[i].getSamples() + j, sizeof(int16Buffer[j]));
                    }
                }
                break;
            }
            case ASIOSTInt24LSB:
                break;
            case ASIOSTInt32LSB:
            {
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer[currentBufferIndex]);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int32Max);
                }
                break;
            }
            case ASIOSTFloat32LSB:
                std::memcpy(buffer[currentBufferIndex], masterTrackAudioBufferView[i].getSamples(), bufferSize * sizeof(float));
                break;
            case ASIOSTFloat64LSB:
            {
                double* doubleBuffer = reinterpret_cast<double*>(buffer[currentBufferIndex]);
                for(int j = 0; j < bufferSize; ++j)
                {
                    doubleBuffer[j] = static_cast<double>(masterTrackAudioBufferView[i][j]);
                }
            }
                break;
            case ASIOSTInt32LSB16:
            {
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer[currentBufferIndex]);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int16Max);
                }
                break;
            }
            case ASIOSTInt32LSB18:
            {
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer[currentBufferIndex]);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int18Max);
                }
                break;
            }
            case ASIOSTInt32LSB20:
            {
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer[currentBufferIndex]);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int20Max);
                }
                break;
            }
            case ASIOSTInt32LSB24:
            {
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer[currentBufferIndex]);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int24Max);
                }
                break;
            }
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
        return nullptr;
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
            ret = 1L;
            break;
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
