#ifndef MUSEC_AUDIO_PLUGIN_VST2SPEAKERGROUPCOLLECTION
#define MUSEC_AUDIO_PLUGIN_VST2SPEAKERGROUPCOLLECTION

#include "audio/device/ISpeakerGroupCollection.hpp"

#include <pluginterfaces/vst2.x/aeffectx.h>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
using namespace Musec::Audio::Device;

class VST2SpeakerGroupCollection: public ISpeakerGroupCollection
{
public:
    VST2SpeakerGroupCollection();
    VST2SpeakerGroupCollection(VstSpeakerArrangement* const speakerArrangements,
        std::uint8_t speakerArrangementCount);
    VST2SpeakerGroupCollection(const VST2SpeakerGroupCollection&) = default;
    VST2SpeakerGroupCollection(VST2SpeakerGroupCollection&&) noexcept = default;
    VST2SpeakerGroupCollection& operator=(VST2SpeakerGroupCollection rhs);
    ~VST2SpeakerGroupCollection() noexcept = default;
public:
    std::uint8_t speakerGroupCount() const override;
    const ISpeakerGroup& speakerGroupAt(std::uint8_t index) const override;
private:
    std::uint8_t speakerArrangementCount_;
    VstSpeakerArrangement* speakerArrangements_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST2SPEAKERGROUPCOLLECTION
