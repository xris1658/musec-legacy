#include "AudioTrack.hpp"

#include <utility>

namespace Musec::Audio::Track
{
const std::list<Musec::Audio::Track::PluginSequence<double>>& AudioTrack::getPluginSequences() const
{
    return pluginSequences_;
}

void AudioTrack::setPluginSequences(const std::list<Musec::Audio::Track::PluginSequence<double>>& pluginSequences)
{
    pluginSequences_ = pluginSequences;
}

void AudioTrack::setPluginSequences(std::list<Musec::Audio::Track::PluginSequence<double>>&& pluginSequences)
{
    pluginSequences_ = std::move(pluginSequences);
}

TrackType AudioTrack::trackType() const
{
    return TrackType::kAudioTrack;
}
}