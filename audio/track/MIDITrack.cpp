#include "MIDITrack.hpp"

namespace Musec::Audio::Track
{
MIDITrack::MIDITrack():
    pluginSequences_()
{
    //
}

const std::vector<Musec::Audio::Track::PluginSequence<double>>& MIDITrack::getPluginSequences() const
{
    return pluginSequences_;
}

void MIDITrack::setPluginSequences(const std::vector<Musec::Audio::Track::PluginSequence<double>>& pluginSequences)
{
    pluginSequences_ = pluginSequences;
}

void MIDITrack::setPluginSequences(std::vector<Musec::Audio::Track::PluginSequence<double>>&& pluginSequences)
{
    pluginSequences_ = std::move(pluginSequences);
}

TrackType MIDITrack::trackType() const
{
    return TrackType::kMIDITrack;
}
}
