#include "VST2SpeakerGroup.hpp"

namespace Musec::Audio::Plugin
{
using namespace Musec::Audio::Device;

VST2SpeakerGroup::VST2SpeakerGroup(const VstSpeakerArrangement& speakerArrangement):
    speakerArrangement_(speakerArrangement)
{
    //
}

QString VST2SpeakerGroup::name() const
{
    return {};
}

uint8_t VST2SpeakerGroup::speakerCount() const
{
    return speakerArrangement_.numChannels;
}

SpeakerGroupType VST2SpeakerGroup::type() const
{
    return
        speakerArrangement_.type == VstSpeakerArrangementType::kSpeakerArrStereo?
            SpeakerGroupType::SpeakerGroupStereo:
        speakerArrangement_.type == VstSpeakerArrangementType::kSpeakerArrMono?
            SpeakerGroupType::SpeakerGroupMono:
            SpeakerGroupType::SpeakerGroupCustom;
}

SpeakerType VST2SpeakerGroup::speakerAt(std::uint8_t index) const
{
    if(speakerArrangement_.type == VstSpeakerArrangementType::kSpeakerArrStereo)
    {
        return index == 0? SpeakerType::SpeakerLeft:
               index == 1? SpeakerType::SpeakerRight:
               SpeakerType::SpeakerInvalid;
    }
    else if(speakerArrangement_.type == VstSpeakerArrangementType::kSpeakerArrMono)
    {
        return index == 0? SpeakerType::SpeakerMono: SpeakerType::SpeakerInvalid;
    }
    return index < speakerCount()? SpeakerType::SpeakerCustom: SpeakerType::SpeakerInvalid;
}

QString VST2SpeakerGroup::speakerNameAt(std::uint8_t index) const
{
    if(index < speakerCount())
    {
        return speakerArrangement_.speakers[index].name;
    }
    else
    {
        return {};
    }
}

bool VST2SpeakerGroup::isMain() const
{
    return true;
}
}