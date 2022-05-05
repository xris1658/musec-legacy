#include "AudioTrack.hpp"

#include <utility>

namespace Musec::Audio::Track
{
AudioTrack::AudioTrack():
    pluginSequences_()
{
    //
}

AudioTrack::~AudioTrack()
{
    pluginSequences_.clear();
}

const std::vector<Musec::Audio::Track::PluginSequence<float>>& AudioTrack::getPluginSequences() const
{
    return pluginSequences_;
}

void AudioTrack::setPluginSequences(const std::vector<Musec::Audio::Track::PluginSequence<float>>& pluginSequences)
{
    pluginSequences_ = pluginSequences;
}

void AudioTrack::setPluginSequences(std::vector<Musec::Audio::Track::PluginSequence<float>>&& pluginSequences)
{
    pluginSequences_ = std::move(pluginSequences);
}

TrackType AudioTrack::trackType() const
{
    return TrackType::kAudioTrack;
}

void AudioTrack::clear()
{
    pluginSequences_.clear();
}
}
