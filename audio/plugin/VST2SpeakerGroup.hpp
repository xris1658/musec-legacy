#ifndef MUSEC_AUDIO_PLUGIN_VST2SPEAKERGROUP
#define MUSEC_AUDIO_PLUGIN_VST2SPEAKERGROUP

#include "audio/device/ISpeakerGroup.hpp"

#include <pluginterfaces/vst2.x/aeffectx.h>

#include <QString>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
using namespace Musec::Audio::Device;
class VST2SpeakerGroup: public ISpeakerGroup
{
public:
    VST2SpeakerGroup(const VstSpeakerArrangement& speakerArrangement);
public:
    QString name() const override;
    uint8_t speakerCount() const override;
    SpeakerGroupType type() const override;
    SpeakerType speakerAt(std::uint8_t index) const override;
    QString speakerNameAt(std::uint8_t index) const override;
    bool isMain() const override;
private:
    const VstSpeakerArrangement& speakerArrangement_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST2SPEAKERGROUP
