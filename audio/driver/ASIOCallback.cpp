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
bool driverSupportsOutputReady;

void onASIOBufferSwitch(long doubleBufferIndex, ASIOBool directProcess)
{
    if(*AppASIODriver())
    {
        std::array<Musec::Audio::Base::AudioBufferView<float>, 2> masterTrackAudioBufferView = {};
        auto bufferSize = Musec::Audio::Driver::getBufferSize().preferredBufferSize;
        masterTrackAudioBufferView[0] = Musec::Audio::Base::AudioBufferView<float>(
                reinterpret_cast<float*>(Musec::Controller::AudioEngineController::AppProject().masterTrackAudioBuffer().data()),
                bufferSize
        );
        masterTrackAudioBufferView[1] = Musec::Audio::Base::AudioBufferView<float>(
                reinterpret_cast<float*>(Musec::Controller::AudioEngineController::AppProject().masterTrackAudioBuffer().data()) + bufferSize,
                bufferSize
        );
        std::array<int, Musec::Audio::Driver::maxInputChannelCount> inputs = {0};
        std::array<int, Musec::Audio::Driver::maxOutputChannelCount> outputs = {0};
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
        Musec::Controller::AudioEngineController::AppProject().process();
        for(int i = 0; i < 2; ++i)
        {
            using int24_t = std::int8_t[3];
            auto buffer = bufferInfoList[outputs[i]].buffers[doubleBufferIndex];
            // buffer[0] 和 buffer[1] 是 ASIO 的双缓冲区地址
            // 有些驱动程序 (e.g. ASIO4ALL）不使用双缓冲，两个地址相同
            // 多数驱动程序（FlexASIO）使用双缓冲，两个地址不同
            switch(channelInfoList[outputs[i]].type)
            {
            // MSB
            case ASIOSTInt16MSB:
            {
                Musec::Audio::Base::clip(masterTrackAudioBufferView[i]);
                std::int16_t* int16Buffer = reinterpret_cast<std::int16_t*>(buffer);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int16Buffer[j] = static_cast<std::int16_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int16Max);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::LittleEndian)
                    {
                        Musec::Util::reverseEndianness(int16Buffer + j, sizeof(std::int16_t));
                    }
                }
                break;
            }
            case ASIOSTInt24MSB:
            {
                Musec::Audio::Base::clip(masterTrackAudioBufferView[i]);
                int24_t* int24Buffer = reinterpret_cast<int24_t*>(buffer);
                std::int32_t temp;
                for(int j = 0; j < bufferSize; ++j)
                {
                    temp = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int24Max);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::BigEndian)
                    {
                        std::memcpy(int24Buffer + j, &temp, sizeof(int24_t));
                    }
                    else
                    {
                        std::memcpy(int24Buffer + j, reinterpret_cast<std::int8_t*>(&temp) + 1, sizeof(int24_t));
                    }
                }
                break;
            }
            case ASIOSTInt32MSB:
            {
                Musec::Audio::Base::clip(masterTrackAudioBufferView[i]);
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int32Max);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::LittleEndian)
                    {
                        Musec::Util::reverseEndianness(int32Buffer + j, sizeof(std::int32_t));
                    }
                }
                break;
            }
            case ASIOSTFloat32MSB:
            {
                if constexpr(Musec::Native::endian() == Musec::Util::Endian::BigEndian)
                {
                    std::memcpy(buffer, masterTrackAudioBufferView[i].getSamples(), bufferSize * sizeof(float));
                }
                else
                {
                    Musec::Util::reverseEndiannessCopy(masterTrackAudioBufferView[i].getSamples(), bufferSize * sizeof(float), buffer);
                }
                break;
            }
            case ASIOSTFloat64MSB:
            {
                double* doubleBuffer = reinterpret_cast<double*>(buffer);
                for(int j = 0; j < bufferSize; ++j)
                {
                    doubleBuffer[j] = static_cast<double>(masterTrackAudioBufferView[i][j]);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::LittleEndian)
                    {
                        Musec::Util::reverseEndianness(doubleBuffer + j, sizeof(double));
                    }
                }
                break;
            }
            case ASIOSTInt32MSB16:
            {
                Musec::Audio::Base::clip(masterTrackAudioBufferView[i]);
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int16Max);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::LittleEndian)
                    {
                        Musec::Util::reverseEndianness(int32Buffer + j, sizeof(std::int32_t));
                    }
                }
                break;
            }
            case ASIOSTInt32MSB18:
            {
                Musec::Audio::Base::clip(masterTrackAudioBufferView[i]);
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int18Max);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::LittleEndian)
                    {
                        Musec::Util::reverseEndianness(int32Buffer + j, sizeof(std::int32_t));
                    }
                }
                break;
            }
            case ASIOSTInt32MSB20:
            {
                Musec::Audio::Base::clip(masterTrackAudioBufferView[i]);
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int20Max);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::LittleEndian)
                    {
                        Musec::Util::reverseEndianness(int32Buffer + j, sizeof(std::int32_t));
                    }
                }
                break;
            }
            case ASIOSTInt32MSB24:
            {
                Musec::Audio::Base::clip(masterTrackAudioBufferView[i]);
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int24Max);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::LittleEndian)
                    {
                        Musec::Util::reverseEndianness(int32Buffer + j, sizeof(std::int32_t));
                    }
                }
                break;
            }
            // LSB
            case ASIOSTInt16LSB:
            {
                Musec::Audio::Base::clip(masterTrackAudioBufferView[i]);
                std::int16_t* int16Buffer = reinterpret_cast<std::int16_t*>(buffer);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int16Buffer[j] = static_cast<std::int16_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int16Max);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::BigEndian)
                    {
                        Musec::Util::reverseEndianness(int16Buffer + j, sizeof(std::int16_t));
                    }
                }
                break;
            }
            case ASIOSTInt24LSB:
            {
                Musec::Audio::Base::clip(masterTrackAudioBufferView[i]);
                int24_t* int24Buffer = reinterpret_cast<int24_t*>(buffer);
                std::int32_t temp;
                for(int j = 0; j < bufferSize; ++j)
                {
                    temp = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int24Max);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::LittleEndian)
                    {
                        std::memcpy(int24Buffer + j, &temp, sizeof(int24_t));
                    }
                    else
                    {
                        std::memcpy(int24Buffer + j, reinterpret_cast<std::int8_t*>(&temp) + 1, sizeof(int24_t));
                    }
                }
                break;
            }
            case ASIOSTInt32LSB:
            {
                Musec::Audio::Base::clip(masterTrackAudioBufferView[i]);
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int32Max);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::BigEndian)
                    {
                        Musec::Util::reverseEndianness(int32Buffer + j, sizeof(std::int32_t));
                    }
                }
                break;
            }
            case ASIOSTFloat32LSB:
            {
                if constexpr(Musec::Native::endian() == Musec::Util::Endian::LittleEndian)
                {
                    std::memcpy(buffer, masterTrackAudioBufferView[i].getSamples(), bufferSize * sizeof(float));
                }
                else
                {
                    Musec::Util::reverseEndiannessCopy(masterTrackAudioBufferView[i].getSamples(), bufferSize * sizeof(float), buffer);
                }
                break;
            }
            case ASIOSTFloat64LSB:
            {
                double* doubleBuffer = reinterpret_cast<double*>(buffer);
                for(int j = 0; j < bufferSize; ++j)
                {
                    doubleBuffer[j] = static_cast<double>(masterTrackAudioBufferView[i][j]);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::BigEndian)
                    {
                        Musec::Util::reverseEndianness(doubleBuffer + j, sizeof(double));
                    }
                }
                break;
            }
            case ASIOSTInt32LSB16:
            {
                Musec::Audio::Base::clip(masterTrackAudioBufferView[i]);
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int16Max);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::BigEndian)
                    {
                        Musec::Util::reverseEndianness(int32Buffer + j, sizeof(std::int32_t));
                    }
                }
                break;
            }
            case ASIOSTInt32LSB18:
            {
                Musec::Audio::Base::clip(masterTrackAudioBufferView[i]);
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int18Max);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::BigEndian)
                    {
                        Musec::Util::reverseEndianness(int32Buffer + j, sizeof(std::int32_t));
                    }
                }
                break;
            }
            case ASIOSTInt32LSB20:
            {
                Musec::Audio::Base::clip(masterTrackAudioBufferView[i]);
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int20Max);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::BigEndian)
                    {
                        Musec::Util::reverseEndianness(int32Buffer + j, sizeof(std::int32_t));
                    }
                }
                break;
            }
            case ASIOSTInt32LSB24:
            {
                Musec::Audio::Base::clip(masterTrackAudioBufferView[i]);
                std::int32_t* int32Buffer = reinterpret_cast<std::int32_t*>(buffer);
                for(int j = 0; j < bufferSize; ++j)
                {
                    int32Buffer[j] = static_cast<std::int32_t>(masterTrackAudioBufferView[i][j] * Musec::Audio::Base::Int24Max);
                    if constexpr(Musec::Native::endian() == Musec::Util::Endian::BigEndian)
                    {
                        Musec::Util::reverseEndianness(int32Buffer + j, sizeof(std::int32_t));
                    }
                }
                break;
            }
            // DSD
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
        if(AppASIODriver() && driverSupportsOutputReady)
        {
            AppASIODriver()->outputReady();
        }
    }
}

ASIOTime* onASIOBufferSwitchTimeInfo(ASIOTime* params,
                                     long doubleBufferIndex,
                                     ASIOBool directProcess)
{
    onASIOBufferSwitch(doubleBufferIndex, directProcess);
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
            ret = 2L;
            break;
    case kAsioSupportsTimeInfo:
        break;
    default:
        ret = 1L;
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
