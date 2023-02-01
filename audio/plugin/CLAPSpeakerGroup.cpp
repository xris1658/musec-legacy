#include "CLAPSpeakerGroup.hpp"

#include <cstring>

namespace Musec::Audio::Plugin
{
using namespace Musec::Audio::Device;

CLAPSpeakerGroup::CLAPSpeakerGroup():
    audioPortInfo_{}
{
}

CLAPSpeakerGroup::CLAPSpeakerGroup(const clap_audio_port_info& audioPortInfo):
    audioPortInfo_(audioPortInfo)
{
}

CLAPSpeakerGroup& CLAPSpeakerGroup::operator=(const CLAPSpeakerGroup& rhs)
{
    std::memcpy(reinterpret_cast<void*>(this), reinterpret_cast<const void*>(&rhs),
        sizeof(CLAPSpeakerGroup));
    return *this;
}

QString CLAPSpeakerGroup::name() const
{
    return QString::fromUtf8(audioPortInfo_.name);
}

uint8_t CLAPSpeakerGroup::channelCount() const
{
    return audioPortInfo_.channel_count;
}

Musec::Audio::Base::ChannelGroupType CLAPSpeakerGroup::type() const
{
    return std::strcmp(audioPortInfo_.port_type, CLAP_PORT_STEREO) == 0? Musec::Audio::Base::ChannelGroupType::Stereo:
        std::strcmp(audioPortInfo_.port_type, CLAP_PORT_MONO) == 0? Musec::Audio::Base::ChannelGroupType::Mono:
        Musec::Audio::Base::ChannelGroupType::Custom;
}

Musec::Audio::Base::ChannelType CLAPSpeakerGroup::speakerAt(std::uint8_t index) const
{
    if(std::strcmp(audioPortInfo_.port_type, CLAP_PORT_STEREO) == 0)
    {
        return index == 0? Musec::Audio::Base::ChannelType::Left:
               index == 1? Musec::Audio::Base::ChannelType::Right:
               Musec::Audio::Base::ChannelType::Invalid;
    }
    else if(std::strcmp(audioPortInfo_.port_type, CLAP_PORT_MONO) == 0)
    {
        return index == 0? Musec::Audio::Base::ChannelType::Center: Musec::Audio::Base::ChannelType::Invalid;
    }
    return index < channelCount()? Musec::Audio::Base::ChannelType::Custom: Musec::Audio::Base::ChannelType::Invalid;
}

QString CLAPSpeakerGroup::speakerNameAt(std::uint8_t index) const
{
    return {};
}

bool CLAPSpeakerGroup::isMain() const
{
    return audioPortInfo_.flags & CLAP_AUDIO_PORT_IS_MAIN;
}
}