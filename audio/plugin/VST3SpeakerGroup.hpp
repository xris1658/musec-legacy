#ifndef MUSEC_AUDIO_PLUGIN_VST3SPEAKERGROUP
#define MUSEC_AUDIO_PLUGIN_VST3SPEAKERGROUP

#include "audio/device/ISpeakerGroup.hpp"

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

class VST3SpeakerGroup: public ISpeakerGroup
{
public:
    VST3SpeakerGroup();
    VST3SpeakerGroup(SpeakerArrangement speakerArrangement, BusInfo busInfo);
    VST3SpeakerGroup(const VST3SpeakerGroup&) = default;
    VST3SpeakerGroup(VST3SpeakerGroup&&) noexcept = default;
    VST3SpeakerGroup& operator=(VST3SpeakerGroup);
    ~VST3SpeakerGroup() noexcept = default;
public:
    QString name() const override;
    std::uint8_t speakerCount() const override;
    SpeakerGroupType type() const override;
    SpeakerType speakerAt(std::uint8_t index) const override;
    QString speakerNameAt(std::uint8_t index) const override;
    bool isMain() const override;
private:
    SpeakerArrangement speakerArrangement_;
    BusInfo busInfo_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST3SPEAKERGROUP
