#include "MIDITrack.hpp"

namespace Musec::Audio::Track
{
MIDITrack::MIDITrack():
    pluginSequences_()
{
    //
}

MIDITrack::~MIDITrack()
{
    pluginSequences_.clear();
}

const std::vector<Musec::Audio::Track::PluginSequence>& MIDITrack::getPluginSequences() const
{
    return pluginSequences_;
}

void MIDITrack::setPluginSequences(const std::vector<Musec::Audio::Track::PluginSequence>& pluginSequences)
{
    pluginSequences_ = pluginSequences;
}

void MIDITrack::setPluginSequences(std::vector<Musec::Audio::Track::PluginSequence>&& pluginSequences)
{
    pluginSequences_ = std::move(pluginSequences);
}

TrackType MIDITrack::trackType() const
{
    return TrackType::kMIDITrack;
}
}
