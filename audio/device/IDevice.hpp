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
template<typename SampleType>
class IDevice
{
public:
    IDevice()
    {
        //
    }
    virtual ~IDevice() noexcept = default;
public:
    virtual std::uint8_t inputCount() const = 0;
    virtual std::uint8_t outputCount() const = 0;
public:
    virtual void process(const Musec::Audio::Base::AudioBufferViews<SampleType>& inputs, const Musec::Audio::Base::AudioBufferViews<SampleType>& outputs) = 0;
};
}
}
}

#endif //MUSEC_AUDIO_DEVICE_IDEVICE
