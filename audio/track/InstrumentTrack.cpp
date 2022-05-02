#include "InstrumentTrack.hpp"

#include <utility>

namespace Musec::Audio::Track
{
InstrumentTrack::InstrumentTrack():
    midiEffectPluginSequences_(),
    instrument_(),
    audioEffectPluginSequences_()
{
    //
}

const std::vector<Musec::Audio::Track::PluginSequence<double>>& InstrumentTrack::getMIDIEffectPluginSequences() const
{
    return midiEffectPluginSequences_;
}

void InstrumentTrack::setMIDIEffectPluginSequences(
    const std::vector<Musec::Audio::Track::PluginSequence<double>>& midiEffectPluginSequences)
{
    midiEffectPluginSequences_ = midiEffectPluginSequences;
}

void InstrumentTrack::setMIDIEffectPluginSequences(
    std::vector<Musec::Audio::Track::PluginSequence<double>>&& midiEffectPluginSequences)
{
    midiEffectPluginSequences_ = std::move(midiEffectPluginSequences);
}

const std::shared_ptr<Musec::Audio::Plugin::IPlugin<double>>& InstrumentTrack::getInstrument() const
{
    return instrument_;
}

void InstrumentTrack::setInstrument(const std::shared_ptr<Musec::Audio::Plugin::IPlugin<double>>& instrument)
{
    instrument_ = instrument;
}

void InstrumentTrack::setInstrument(std::shared_ptr<Musec::Audio::Plugin::IPlugin<double>>&& instrument)
{
    instrument_ = std::move(instrument);
}

const std::vector<Musec::Audio::Track::PluginSequence<double>>& InstrumentTrack::getAudioEffectPluginSequences() const
{
    return audioEffectPluginSequences_;
}

void InstrumentTrack::setAudioEffectPluginSequences(
    const std::vector<Musec::Audio::Track::PluginSequence<double>>& audioEffectPluginSequences)
{
    audioEffectPluginSequences_ = audioEffectPluginSequences;
}

void InstrumentTrack::setAudioEffectPluginSequences(
    std::vector<Musec::Audio::Track::PluginSequence<double>>&& audioEffectPluginSequences)
{
    audioEffectPluginSequences_ = std::move(audioEffectPluginSequences);
}

TrackType InstrumentTrack::trackType() const
{
    return TrackType::kInstrumentTrack;
}
}
