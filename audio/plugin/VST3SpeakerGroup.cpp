#include "VST3SpeakerGroup.hpp"

namespace Musec::Audio::Plugin
{
using namespace Musec::Audio::Device;

VST3SpeakerGroup::VST3SpeakerGroup():
    speakerArrangement_(SpeakerArr::kEmpty),
    busInfo_()
{
}

VST3SpeakerGroup::VST3SpeakerGroup(SpeakerArrangement speakerArrangement, BusInfo busInfo):
    speakerArrangement_(speakerArrangement),
    busInfo_(busInfo)
{
}

VST3SpeakerGroup& VST3SpeakerGroup::operator=(VST3SpeakerGroup rhs)
{
    std::swap(*this, rhs);
    return *this;
}

QString VST3SpeakerGroup::name() const
{
    return QString::fromUtf16(busInfo_.name);
}

std::uint8_t VST3SpeakerGroup::channelCount() const
{
    // Alternative solution
    // return SpeakerArr::getChannelCount(speakerArrangement_);
    return busInfo_.channelCount;
}

Musec::Audio::Base::ChannelGroupType VST3SpeakerGroup::type() const
{
    return speakerArrangement_ == SpeakerArr::kStereo? Musec::Audio::Base::ChannelGroupType::Stereo:
        speakerArrangement_ == SpeakerArr::kMono? Musec::Audio::Base::ChannelGroupType::Mono:
        Musec::Audio::Base::ChannelGroupType::Custom;
}

Musec::Audio::Base::ChannelType VST3SpeakerGroup::speakerAt(std::uint8_t index) const
{
    auto speaker = SpeakerArr::getSpeaker(speakerArrangement_, index);
    if(speaker == Steinberg::Vst::kSpeakerL)
    {
        return Musec::Audio::Base::ChannelType::Left;
    }
    else if(speaker == Steinberg::Vst::kSpeakerR)
    {
        return Musec::Audio::Base::ChannelType::Right;
    }
    else if(speaker == 0)
    {
        return Musec::Audio::Base::ChannelType::Invalid;
    }
    return index < channelCount()? Musec::Audio::Base::ChannelType::Custom: Musec::Audio::Base::ChannelType::Invalid;
}

QString VST3SpeakerGroup::speakerNameAt(std::uint8_t index) const
{
    return SpeakerArr::getSpeakerShortName(speakerArrangement_, index);
}

bool VST3SpeakerGroup::isMain() const
{
    return busInfo_.busType == BusTypes::kMain;
}
}