#include "AudioTrack.hpp"

#include <utility>

namespace Musec::Audio::Track
{
AudioTrack::AudioTrack():
    pluginSequences_()
{
    //
}

const std::vector<Musec::Audio::Track::PluginSequence<double>>& AudioTrack::getPluginSequences() const
{
    return pluginSequences_;
}

void AudioTrack::setPluginSequences(const std::vector<Musec::Audio::Track::PluginSequence<double>>& pluginSequences)
{
    pluginSequences_ = pluginSequences;
}

void AudioTrack::setPluginSequences(std::vector<Musec::Audio::Track::PluginSequence<double>>&& pluginSequences)
{
    pluginSequences_ = std::move(pluginSequences);
}

TrackType AudioTrack::trackType() const
{
    return TrackType::kAudioTrack;
}
}
