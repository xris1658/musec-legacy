#ifndef MUSEC_AUDIO_DEVICE_IDEVICE
#define MUSEC_AUDIO_DEVICE_IDEVICE

#include "audio/base/AudioBufferView.hpp"

#include <vector>

namespace Musec
{
namespace Audio
{
namespace Device
{
class IDevice
{
public:
    IDevice()
    {
        //
    }
    virtual ~IDevice() = default;
public:
    virtual std::uint8_t inputCount() const = 0;
    virtual std::uint8_t outputCount() const = 0;
public:
    virtual void process(Musec::Audio::Base::AudioBufferView<float>* input, int inputCount,
        Musec::Audio::Base::AudioBufferView<float>* output, int outputCount) = 0;
};
}
}
}

#endif //MUSEC_AUDIO_DEVICE_IDEVICE
