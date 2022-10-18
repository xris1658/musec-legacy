#ifndef MUSEC_AUDIO_DEVICE_IDEVICE
#define MUSEC_AUDIO_DEVICE_IDEVICE

#include "audio/base/AudioBufferView.hpp"
#include "audio/device/ISpeakerGroupCollection.hpp"
#include "native/CompilerSpecific.hpp"

#include <vector>

namespace Musec
{
namespace Audio
{
namespace Device
{
MUSEC_INTERFACE IDevice
{
public:
    IDevice()
    {
        //
    }
    virtual ~IDevice() = default;
public:
    virtual std::uint8_t audioInputCount() const = 0;
    virtual std::uint8_t audioOutputCount() const = 0;
    virtual const ISpeakerGroupCollection& audioInputSpeakerGroupCollection() const = 0;
    virtual const ISpeakerGroupCollection& audioOutputSpeakerGroupCollection() const = 0;
public:
    virtual void process(Musec::Audio::Base::AudioBufferView<float>* input, int inputCount,
        Musec::Audio::Base::AudioBufferView<float>* output, int outputCount) = 0;
};
}
}
}

#endif //MUSEC_AUDIO_DEVICE_IDEVICE
