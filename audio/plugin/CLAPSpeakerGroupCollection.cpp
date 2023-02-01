#include "CLAPSpeakerGroupCollection.hpp"

#include "audio/plugin/CLAPSpeakerGroup.hpp"

#include <clap/ext/audio-ports.h>

namespace Musec::Audio::Plugin
{
CLAPSpeakerGroupCollection::CLAPSpeakerGroupCollection():
    plugin_(nullptr),
    audioPorts_(nullptr)
{

}

CLAPSpeakerGroupCollection::CLAPSpeakerGroupCollection(const clap_plugin* const plugin,
    const clap_plugin_audio_ports* const audioPorts, bool isInput):
    plugin_(plugin),
    audioPorts_(audioPorts),
    isInput_(isInput)
{
}

CLAPSpeakerGroupCollection& CLAPSpeakerGroupCollection::operator=(CLAPSpeakerGroupCollection rhs)
{
    plugin_ = rhs.plugin_;
    audioPorts_ = rhs.audioPorts_;
    isInput_ = rhs.isInput_;
    return *this;
}

uint8_t CLAPSpeakerGroupCollection::channelGroupCount() const
{
    return audioPorts_->count(plugin_, isInput_);
}

const IChannelGroup& CLAPSpeakerGroupCollection::channelGroupAt(std::uint8_t index) const
{
    CLAPSpeakerGroup speakerGroup;
    audioPorts_->get(plugin_, index, isInput_, &(speakerGroup.audioPortInfo_));
    return speakerGroup;
}
}