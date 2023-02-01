#include "VST3SpeakerGroupCollection.hpp"

#include "audio/plugin/VST3SpeakerGroup.hpp"

namespace Musec::Audio::Plugin
{
using namespace Steinberg::Vst;

VST3SpeakerGroupCollection::VST3SpeakerGroupCollection():
    component_(nullptr),
    audioProcessor_(nullptr)
{
}

VST3SpeakerGroupCollection::VST3SpeakerGroupCollection(IComponent* component,
    IAudioProcessor* audioProcessor, BusDirections busDirection):
    component_(component),
    audioProcessor_(audioProcessor),
    busDirection_(busDirection)
{
    //
}

VST3SpeakerGroupCollection& VST3SpeakerGroupCollection::operator=(VST3SpeakerGroupCollection rhs)
{
    component_ = rhs.component_;
    audioProcessor_ = rhs.audioProcessor_;
    busDirection_ = rhs.busDirection_;
    return *this;
}

std::uint8_t VST3SpeakerGroupCollection::channelGroupCount() const
{
    return component_->getBusCount(MediaTypes::kAudio, busDirection_);
}

const IChannelGroup& VST3SpeakerGroupCollection::channelGroupAt(std::uint8_t index) const
{
    SpeakerArrangement speakerArrangement;
    BusInfo busInfo;
    audioProcessor_->getBusArrangement(busDirection_, index, speakerArrangement);
    component_->getBusInfo(MediaTypes::kAudio, busDirection_, index, busInfo);
    return Musec::Audio::Plugin::VST3SpeakerGroup(speakerArrangement, busInfo);
}
}