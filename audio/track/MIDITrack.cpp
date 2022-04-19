#include "MIDITrack.hpp"

namespace Musec::Audio::Track
{
const std::list<Musec::Audio::Track::PluginSequence<double>>& MIDITrack::getPluginSequences() const
{
    return pluginSequences_;
}

void MIDITrack::setPluginSequences(const std::list<Musec::Audio::Track::PluginSequence<double>>& pluginSequences)
{
    pluginSequences_ = pluginSequences;
}

void MIDITrack::setPluginSequences(std::list<Musec::Audio::Track::PluginSequence<double>>&& pluginSequences)
{
    pluginSequences_ = std::move(pluginSequences);
}

TrackType MIDITrack::trackType() const
{
    return TrackType::kMIDITrack;
}
}