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

uint8_t CLAPSpeakerGroup::speakerCount() const
{
    return audioPortInfo_.channel_count;
}

SpeakerGroupType CLAPSpeakerGroup::type() const
{
    return std::strcmp(audioPortInfo_.port_type, CLAP_PORT_STEREO) == 0? SpeakerGroupType::SpeakerGroupStereo:
        std::strcmp(audioPortInfo_.port_type, CLAP_PORT_MONO) == 0? SpeakerGroupType::SpeakerGroupMono:
        SpeakerGroupType::SpeakerGroupCustom;
}

SpeakerType CLAPSpeakerGroup::speakerAt(std::uint8_t index) const
{
    if(std::strcmp(audioPortInfo_.port_type, CLAP_PORT_STEREO) == 0)
    {
        return index == 0? SpeakerType::SpeakerLeft:
               index == 1? SpeakerType::SpeakerRight:
               SpeakerType::SpeakerInvalid;
    }
    else if(std::strcmp(audioPortInfo_.port_type, CLAP_PORT_MONO) == 0)
    {
        return index == 0? SpeakerType::SpeakerMono: SpeakerType::SpeakerInvalid;
    }
    return index < speakerCount()? SpeakerType::SpeakerCustom: SpeakerType::SpeakerInvalid;
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