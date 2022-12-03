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

std::uint8_t VST3SpeakerGroup::speakerCount() const
{
    // Alternative solution
    // return SpeakerArr::getChannelCount(speakerArrangement_);
    return busInfo_.channelCount;
}

SpeakerGroupType VST3SpeakerGroup::type() const
{
    return speakerArrangement_ == SpeakerArr::kStereo? SpeakerGroupType::SpeakerGroupStereo:
        speakerArrangement_ == SpeakerArr::kMono? SpeakerGroupType::SpeakerGroupMono:
        SpeakerGroupType::SpeakerGroupCustom;
}

SpeakerType VST3SpeakerGroup::speakerAt(std::uint8_t index) const
{
    auto speaker = SpeakerArr::getSpeaker(speakerArrangement_, index);
    if(speaker == Steinberg::Vst::kSpeakerL)
    {
        return SpeakerType::SpeakerLeft;
    }
    else if(speaker == Steinberg::Vst::kSpeakerR)
    {
        return SpeakerType::SpeakerRight;
    }
    else if(speaker == 0)
    {
        return SpeakerType::SpeakerInvalid;
    }
    return index < speakerCount()? SpeakerType::SpeakerCustom: SpeakerType::SpeakerInvalid;
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