#ifndef MUSEC_AUDIO_DEVICE_IDEVICE
#define MUSEC_AUDIO_DEVICE_IDEVICE

#include "audio/base/AudioBufferView.hpp"
#include "audio/device/IChannelGroupCollection.hpp"
#include "native/CompilerSpecific.hpp"

#include <vector>

namespace Musec
{
namespace Audio
{
namespace Device
{
template<typename SampleType>
struct AudioProcessData
{
    int inputGroupCount = 0;
    int outputGroupCount = 0;
    int singleBufferSize = 0;
    int* inputCounts = nullptr;
    int* outputCounts = nullptr;
    SampleType*** inputs = nullptr;
    SampleType*** outputs = nullptr;
};

MUSEC_INTERFACE IDevice
{
public:
    IDevice()
    {
        //
    }
    virtual ~IDevice() = default;
public:
    // Audio input channel count
    virtual std::uint8_t audioInputCount() const = 0;
    // Audio output channel count
    virtual std::uint8_t audioOutputCount() const = 0;
    virtual const IChannelGroupCollection& audioInputSpeakerGroupCollection() const = 0;
    virtual const IChannelGroupCollection& audioOutputSpeakerGroupCollection() const = 0;
public:
    virtual void process(Musec::Audio::Base::AudioBufferView<float>* input, int inputCount,
        Musec::Audio::Base::AudioBufferView<float>* output, int outputCount) = 0;
    // Implementaion of this function should NOT check bounds. It's the user's sole responsibility
    // to match `audioProcessData` with the speaker arrangement of the device.
    virtual void process(const AudioProcessData<float>& audioProcessData) = 0;
public:
    virtual int latency() = 0;
};
}
}
}

#endif //MUSEC_AUDIO_DEVICE_IDEVICE
