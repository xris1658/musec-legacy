#ifndef MUSEC_AUDIO_PLUGIN_CLAPSPEAKERGROUPCOLLECTION
#define MUSEC_AUDIO_PLUGIN_CLAPSPEAKERGROUPCOLLECTION

#include "audio/device/IChannelGroupCollection.hpp"

#include <clap/plugin.h>
#include <clap/ext/audio-ports.h>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
using namespace Musec::Audio::Device;
class CLAPSpeakerGroupCollection: public IChannelGroupCollection
{
public:
    CLAPSpeakerGroupCollection();
    CLAPSpeakerGroupCollection(const clap_plugin* const plugin,
        const clap_plugin_audio_ports* const audioPorts,
        bool isInput);
    CLAPSpeakerGroupCollection(const CLAPSpeakerGroupCollection&) = default;
    CLAPSpeakerGroupCollection(CLAPSpeakerGroupCollection&&) noexcept = default;
    CLAPSpeakerGroupCollection& operator=(CLAPSpeakerGroupCollection rhs);
    ~CLAPSpeakerGroupCollection() noexcept = default;
public:
    std::uint8_t channelGroupCount() const override;
    const IChannelGroup& channelGroupAt(std::uint8_t index) const override;
private:
    const clap_plugin* plugin_;
    const clap_plugin_audio_ports* audioPorts_;
    bool isInput_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_CLAPSPEAKERGROUPCOLLECTION
