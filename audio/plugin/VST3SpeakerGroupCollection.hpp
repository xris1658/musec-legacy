#ifndef MUSEC_AUDIO_PLUGIN_VST3SPEAKERGROUPCOLLECTION
#define MUSEC_AUDIO_PLUGIN_VST3SPEAKERGROUPCOLLECTION

#include "audio/device/ISpeakerGroupCollection.hpp"

#include <pluginterfaces/vst/vstspeaker.h>
#include <pluginterfaces/vst/vsttypes.h>
#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include <pluginterfaces/vst/ivstcomponent.h>

#include <vector>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
using namespace Musec::Audio::Device;

using namespace Steinberg::Vst;

class VST3SpeakerGroupCollection: public ISpeakerGroupCollection
{
public:
    VST3SpeakerGroupCollection();
    VST3SpeakerGroupCollection(IComponent* component, IAudioProcessor* audioProcessor,
        BusDirections busDirection);
    VST3SpeakerGroupCollection(const VST3SpeakerGroupCollection&) = default;
    VST3SpeakerGroupCollection(VST3SpeakerGroupCollection&&) noexcept = default;
    VST3SpeakerGroupCollection& operator=(VST3SpeakerGroupCollection rhs);
    ~VST3SpeakerGroupCollection() noexcept = default;
public:
    std::uint8_t speakerGroupCount() const override;
    const ISpeakerGroup& speakerGroupAt(std::uint8_t index) const override;
private:
    IComponent* component_;
    IAudioProcessor* audioProcessor_;
    BusDirection busDirection_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST3SPEAKERGROUPCOLLECTION
