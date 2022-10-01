#include "VST2SpeakerGroupCollection.hpp"

#include "audio/plugin/VST2SpeakerGroup.hpp"

namespace Musec::Audio::Plugin
{
VST2SpeakerGroupCollection::VST2SpeakerGroupCollection():
    speakerArrangementCount_(0),
    speakerArrangements_(nullptr)
{
}

VST2SpeakerGroupCollection::VST2SpeakerGroupCollection(
    VstSpeakerArrangement* const speakerArrangements,
    std::uint8_t speakerArrangementCount):
    speakerArrangementCount_(speakerArrangements_? 1: 0),
    speakerArrangements_(speakerArrangements)
{
}

VST2SpeakerGroupCollection& VST2SpeakerGroupCollection::operator=(VST2SpeakerGroupCollection rhs)
{
    speakerArrangementCount_ = rhs.speakerArrangementCount_;
    speakerArrangements_ = rhs.speakerArrangements_;
    return *this;
}

std::uint8_t VST2SpeakerGroupCollection::speakerGroupCount() const
{
    return speakerArrangementCount_;
}

const Musec::Audio::Device::ISpeakerGroup& VST2SpeakerGroupCollection::speakerGroupAt(std::uint8_t index) const
{
    return Musec::Audio::Plugin::VST2SpeakerGroup(speakerArrangements_[index]);
}
}