#include "ASIOCallback.hpp"

#include "audio/base/Constants.hpp"
#include "audio/driver/ASIODriver.hpp"
#include "controller/AudioEngineController.hpp"
#include "event/EventBase.hpp"
#include "native/Native.hpp"
#include "util/Endian.hpp"
#include "util/Stopwatch.hpp"

namespace Musec::Audio::Driver
{
bool driverSupportsOutputReady;

namespace Impl
{
void onASIOBufferSwitch(long doubleBufferIndex, ASIOBool directProcess)
{
    using namespace Musec::Audio::Base;
    std::array<AudioBufferView<float>, 2> masterTrackAudioBufferView = {};
    auto bufferSize = getBufferSize().preferredBufferSize;
    masterTrackAudioBufferView[0] = AudioBufferView<float>(
            reinterpret_cast<float*>(Controller::AudioEngineController::AppProject().masterTrackAudioBuffer().data()),
            bufferSize
    );
    masterTrackAudioBufferView[1] = AudioBufferView<float>(
            reinterpret_cast<float*>(Controller::AudioEngineController::AppProject().masterTrackAudioBuffer().data()) + bufferSize,
            bufferSize
    );
    std::array<int, maxInputChannelCount> inputs = {0};
    std::array<int, maxOutputChannelCount> outputs = {0};
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
        // auto buffer = bufferInfoList[inputs[i]].buffers;
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
    Controller::AudioEngineController::AppProject().process();
    for(int i = 0; i < 2; ++i)
    {
        using int24_t = int8_t[3];
        auto buffer = bufferInfoList[outputs[i]].buffers[doubleBufferIndex];
        // buffer[0] and buffer[1] are the double buffers of ASIO
        // Some drivers (e.g. ASIO4ALL) don't use double buffering, and buffer[0] == buffer[1]
        // Most drivers use double buffering, thus buffer[0] != buffer[1]
        switch(channelInfoList[outputs[i]].type)
        {
        // MSB
        case ASIOSTInt16MSB:
        {
            clip(masterTrackAudioBufferView[i]);
            auto* int16Buffer = reinterpret_cast<int16_t*>(buffer);
            for(int j = 0; j < bufferSize; ++j)
            {
                int16Buffer[j] = static_cast<int16_t>(masterTrackAudioBufferView[i][j] * Int16Max);
                if constexpr(Musec::Native::endian() == Musec::Util::Endian::LittleEndian)
                {
                    Musec::Util::reverseEndianness(int16Buffer + j, sizeof(int16_t));
                }
            }
            break;
        }
        case ASIOSTInt24MSB:
        {
            clip(masterTrackAudioBufferView[i]);
            auto* int24Buffer = reinterpret_cast<int24_t*>(buffer);
            int32_t temp;
            for(int j = 0; j < bufferSize; ++j)
            {
                temp = static_cast<int32_t>(masterTrackAudioBufferView[i][j] * Int24Max);
                if constexpr(Native::endian() == Musec::Util::Endian::BigEndian)
                {
                    memcpy(int24Buffer + j, &temp, sizeof(int24_t));
                }
                else
                {
                    memcpy(int24Buffer + j, reinterpret_cast<int8_t*>(&temp) + 1, sizeof(int24_t));
                }
            }
            break;
        }
        case ASIOSTInt32MSB:
        {
            clip(masterTrackAudioBufferView[i]);
            auto* int32Buffer = reinterpret_cast<int32_t*>(buffer);
            for(int j = 0; j < bufferSize; ++j)
            {
                int32Buffer[j] = static_cast<int32_t>(masterTrackAudioBufferView[i][j] * Int32Max);
                if constexpr(Native::endian() == Musec::Util::Endian::LittleEndian)
                {
                    Musec::Util::reverseEndianness(int32Buffer + j, sizeof(int32_t));
                }
            }
            break;
        }
        case ASIOSTFloat32MSB:
        {
            if constexpr(Native::endian() == Musec::Util::Endian::BigEndian)
            {
                memcpy(buffer, masterTrackAudioBufferView[i].getSamples(), bufferSize * sizeof(float));
            }
            else
            {
                Musec::Util::reverseEndiannessCopy(masterTrackAudioBufferView[i].getSamples(), bufferSize * sizeof(float), buffer);
            }
            break;
        }
        case ASIOSTFloat64MSB:
        {
            auto* doubleBuffer = reinterpret_cast<double*>(buffer);
            for(int j = 0; j < bufferSize; ++j)
            {
                doubleBuffer[j] = static_cast<double>(masterTrackAudioBufferView[i][j]);
                if constexpr(Native::endian() == Musec::Util::Endian::LittleEndian)
                {
                    Musec::Util::reverseEndianness(doubleBuffer + j, sizeof(double));
                }
            }
            break;
        }
        case ASIOSTInt32MSB16:
        {
            clip(masterTrackAudioBufferView[i]);
            auto* int32Buffer = reinterpret_cast<int32_t*>(buffer);
            for(int j = 0; j < bufferSize; ++j)
            {
                int32Buffer[j] = static_cast<int32_t>(masterTrackAudioBufferView[i][j] * Int16Max);
                if constexpr(Native::endian() == Musec::Util::Endian::LittleEndian)
                {
                    Musec::Util::reverseEndianness(int32Buffer + j, sizeof(int32_t));
                }
            }
            break;
        }
        case ASIOSTInt32MSB18:
        {
            clip(masterTrackAudioBufferView[i]);
            auto* int32Buffer = reinterpret_cast<int32_t*>(buffer);
            for(int j = 0; j < bufferSize; ++j)
            {
                int32Buffer[j] = static_cast<int32_t>(masterTrackAudioBufferView[i][j] * Int18Max);
                if constexpr(Native::endian() == Musec::Util::Endian::LittleEndian)
                {
                    Musec::Util::reverseEndianness(int32Buffer + j, sizeof(int32_t));
                }
            }
            break;
        }
        case ASIOSTInt32MSB20:
        {
            clip(masterTrackAudioBufferView[i]);
            auto* int32Buffer = reinterpret_cast<int32_t*>(buffer);
            for(int j = 0; j < bufferSize; ++j)
            {
                int32Buffer[j] = static_cast<int32_t>(masterTrackAudioBufferView[i][j] * Int20Max);
                if constexpr(Native::endian() == Musec::Util::Endian::LittleEndian)
                {
                    Musec::Util::reverseEndianness(int32Buffer + j, sizeof(int32_t));
                }
            }
            break;
        }
        case ASIOSTInt32MSB24:
        {
            clip(masterTrackAudioBufferView[i]);
            auto* int32Buffer = reinterpret_cast<int32_t*>(buffer);
            for(int j = 0; j < bufferSize; ++j)
            {
                int32Buffer[j] = static_cast<int32_t>(masterTrackAudioBufferView[i][j] * Int24Max);
                if constexpr(Native::endian() == Musec::Util::Endian::LittleEndian)
                {
                    Musec::Util::reverseEndianness(int32Buffer + j, sizeof(int32_t));
                }
            }
            break;
        }
        // LSB
        case ASIOSTInt16LSB:
        {
            clip(masterTrackAudioBufferView[i]);
            auto* int16Buffer = reinterpret_cast<int16_t*>(buffer);
            for(int j = 0; j < bufferSize; ++j)
            {
                int16Buffer[j] = static_cast<int16_t>(masterTrackAudioBufferView[i][j] * Int16Max);
                if constexpr(Native::endian() == Musec::Util::Endian::BigEndian)
                {
                    Musec::Util::reverseEndianness(int16Buffer + j, sizeof(int16_t));
                }
            }
            break;
        }
        case ASIOSTInt24LSB:
        {
            clip(masterTrackAudioBufferView[i]);
            auto* int24Buffer = reinterpret_cast<int24_t*>(buffer);
            int32_t temp;
            for(int j = 0; j < bufferSize; ++j)
            {
                temp = static_cast<int32_t>(masterTrackAudioBufferView[i][j] * Int24Max);
                if constexpr(Native::endian() == Musec::Util::Endian::LittleEndian)
                {
                    memcpy(int24Buffer + j, &temp, sizeof(int24_t));
                }
                else
                {
                    memcpy(int24Buffer + j, reinterpret_cast<int8_t*>(&temp) + 1, sizeof(int24_t));
                }
            }
            break;
        }
        case ASIOSTInt32LSB:
        {
            clip(masterTrackAudioBufferView[i]);
            auto* int32Buffer = reinterpret_cast<int32_t*>(buffer);
            for(int j = 0; j < bufferSize; ++j)
            {
                int32Buffer[j] = static_cast<int32_t>(masterTrackAudioBufferView[i][j] * Int32Max);
                if constexpr(Native::endian() == Musec::Util::Endian::BigEndian)
                {
                    Musec::Util::reverseEndianness(int32Buffer + j, sizeof(int32_t));
                }
            }
            break;
        }
        case ASIOSTFloat32LSB:
        {
            if constexpr(Native::endian() == Musec::Util::Endian::LittleEndian)
            {
                memcpy(buffer, masterTrackAudioBufferView[i].getSamples(), bufferSize * sizeof(float));
            }
            else
            {
                Musec::Util::reverseEndiannessCopy(masterTrackAudioBufferView[i].getSamples(), bufferSize * sizeof(float), buffer);
            }
            break;
        }
        case ASIOSTFloat64LSB:
        {
            auto* doubleBuffer = reinterpret_cast<double*>(buffer);
            for(int j = 0; j < bufferSize; ++j)
            {
                doubleBuffer[j] = static_cast<double>(masterTrackAudioBufferView[i][j]);
                if constexpr(Native::endian() == Musec::Util::Endian::BigEndian)
                {
                    Musec::Util::reverseEndianness(doubleBuffer + j, sizeof(double));
                }
            }
            break;
        }
        case ASIOSTInt32LSB16:
        {
            clip(masterTrackAudioBufferView[i]);
            auto* int32Buffer = reinterpret_cast<int32_t*>(buffer);
            for(int j = 0; j < bufferSize; ++j)
            {
                int32Buffer[j] = static_cast<int32_t>(masterTrackAudioBufferView[i][j] * Int16Max);
                if constexpr(Native::endian() == Musec::Util::Endian::BigEndian)
                {
                    Musec::Util::reverseEndianness(int32Buffer + j, sizeof(int32_t));
                }
            }
            break;
        }
        case ASIOSTInt32LSB18:
        {
            clip(masterTrackAudioBufferView[i]);
            auto* int32Buffer = reinterpret_cast<int32_t*>(buffer);
            for(int j = 0; j < bufferSize; ++j)
            {
                int32Buffer[j] = static_cast<int32_t>(masterTrackAudioBufferView[i][j] * Int18Max);
                if constexpr(Native::endian() == Musec::Util::Endian::BigEndian)
                {
                    Musec::Util::reverseEndianness(int32Buffer + j, sizeof(int32_t));
                }
            }
            break;
        }
        case ASIOSTInt32LSB20:
        {
            clip(masterTrackAudioBufferView[i]);
            auto* int32Buffer = reinterpret_cast<int32_t*>(buffer);
            for(int j = 0; j < bufferSize; ++j)
            {
                int32Buffer[j] = static_cast<int32_t>(masterTrackAudioBufferView[i][j] * Int20Max);
                if constexpr(Native::endian() == Musec::Util::Endian::BigEndian)
                {
                    Musec::Util::reverseEndianness(int32Buffer + j, sizeof(int32_t));
                }
            }
            break;
        }
        case ASIOSTInt32LSB24:
        {
            clip(masterTrackAudioBufferView[i]);
            auto* int32Buffer = reinterpret_cast<int32_t*>(buffer);
            for(int j = 0; j < bufferSize; ++j)
            {
                int32Buffer[j] = static_cast<int32_t>(masterTrackAudioBufferView[i][j] * Int24Max);
                if constexpr(Native::endian() == Musec::Util::Endian::BigEndian)
                {
                    Musec::Util::reverseEndianness(int32Buffer + j, sizeof(int32_t));
                }
            }
            break;
        }
        // DSD
        case ASIOSTDSDInt8LSB1:
        case ASIOSTDSDInt8MSB1:
        case ASIOSTDSDInt8NER8:
        case ASIOSTLastEntry:
        default:
            break;
        }
    }
    if(AppASIODriver() && driverSupportsOutputReady)
    {
        AppASIODriver()->outputReady();
    }
}
}

double cpuUsage = 0.0;

void onASIOBufferSwitchAndUpdateTime(long doubleBufferIndex, ASIOBool directProcess)
{
    auto timeInNanoSecond = Musec::Util::stopwatchVoid<long, ASIOBool>(
        Impl::onASIOBufferSwitch,
        std::forward<long>(doubleBufferIndex), std::forward<ASIOBool>(directProcess));
    // Do I have to retrieve these values every time?
    auto blockSize = getBufferSize().preferredBufferSize;
    auto sampleRate = getSampleRate();
    auto blockTimeInNanosecond = blockSize * 1e9 / sampleRate;
    cpuUsage = timeInNanoSecond / blockTimeInNanosecond;
}

void onASIOBufferSwitch(long doubleBufferIndex, ASIOBool directProcess)
{
    if(AppASIODriver())
    {
#if NATIVE_INT64
        ASIOSamples samples = 0LL;
        ASIOTimeStamp timeStamp = 0LL;
#else
        ASIOSamples samples {0UL, 0UL};
        ASIOTimeStamp timeStamp {0UL, 0UL};
#endif
        AppASIODriver()->getSamplePosition(&samples, &timeStamp);
#if NATIVE_INT64
        auto systemTime = timeStamp;
#else
        auto systemTime = timeStamp.hi * 0x100000000LL + timeStamp.lo;
#endif
        // TODO: Use systemTime to determine whether the driver has entered steady-state
        onASIOBufferSwitchAndUpdateTime(doubleBufferIndex, directProcess);
    }
}

ASIOTime* onASIOBufferSwitchTimeInfo(ASIOTime* params,
                                     long doubleBufferIndex,
                                     ASIOBool directProcess)
{
    if(AppASIODriver())
    {
#if NATIVE_INT64
        std::int64_t systemTime = params->timeInfo.systemTime;
#else
        std::int64_t systemTime = params->timeInfo.systemTime.hi * 0x100000000LL + params->timeInfo.systemTime.lo;
#endif
        // TODO: Use systemTime to determine whether the driver has entered steady-state
        onASIOBufferSwitchAndUpdateTime(doubleBufferIndex, directProcess);
        return params;
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
    case kAsioEngineVersion:
        return 2L;
    case kAsioSupportsTimeInfo:
        return 1L;
    case kAsioResetRequest:
    case kAsioBufferSizeChange:
        Musec::Event::eventHandler->resetASIODriver();
        return 1L;
    default:
        return 1L;
    }
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
