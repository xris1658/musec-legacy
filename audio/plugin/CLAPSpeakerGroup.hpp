#ifndef MUSEC_AUDIO_PLUGIN_CLAPSPEAKERGROUP
#define MUSEC_AUDIO_PLUGIN_CLAPSPEAKERGROUP

#include "audio/device/ISpeakerGroup.hpp"
#include "audio/plugin/CLAPSpeakerGroupCollection.hpp"

#include <clap/ext/audio-ports.h>

#include <QString>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
using namespace Musec::Audio::Device;

class CLAPSpeakerGroup: public ISpeakerGroup
{
    friend class CLAPSpeakerGroupCollection;
public:
    CLAPSpeakerGroup();
    CLAPSpeakerGroup(const clap_audio_port_info& audioPortInfo);
    CLAPSpeakerGroup(const CLAPSpeakerGroup&) = default;
    // // The move operation is absent since `clap_audio_port_info` has a
    // // stack array of 256 `char`s.
    // CLAPSpeakerGroup(CLAPSpeakerGroup&&) noexcept = default;
    CLAPSpeakerGroup& operator=(const CLAPSpeakerGroup&);
    ~CLAPSpeakerGroup() noexcept = default;
public:
    QString name() const override;
    uint8_t speakerCount() const override;
    SpeakerGroupType type() const override;
    SpeakerType speakerAt(std::uint8_t index) const override;
    QString speakerNameAt(std::uint8_t index) const override;
    bool isMain() const override;
private:
    clap_audio_port_info audioPortInfo_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_CLAPSPEAKERGROUP
