#ifndef MUSEC_AUDIO_TRACK_INSTRUMENTTRACK
#define MUSEC_AUDIO_TRACK_INSTRUMENTTRACK

#include "audio/track/PluginSequence.hpp"
#include "audio/track/ITrack.hpp"

#include <vector>

namespace Musec
{
namespace Audio
{
namespace Track
{
class InstrumentTrack: public Musec::Audio::Track::ITrack
{
public:
    InstrumentTrack();
public:
    const std::vector<Musec::Audio::Track::PluginSequence<double>>& getMIDIEffectPluginSequences() const;
    void setMIDIEffectPluginSequences(
        const std::vector<Musec::Audio::Track::PluginSequence<double>>& midiEffectPluginSequences);
    void setMIDIEffectPluginSequences(
        std::vector<Musec::Audio::Track::PluginSequence<double>>&& midiEffectPluginSequences);
    const std::shared_ptr<Musec::Audio::Plugin::IPlugin<double>>& getInstrument() const;
    void setInstrument(const std::shared_ptr<Musec::Audio::Plugin::IPlugin<double>>& instrument);
    void setInstrument(std::shared_ptr<Musec::Audio::Plugin::IPlugin<double>>&& instrument);
    const std::vector<Musec::Audio::Track::PluginSequence<double>>& getAudioEffectPluginSequences() const;
    void setAudioEffectPluginSequences(
        const std::vector<Musec::Audio::Track::PluginSequence<double>>& audioEffectPluginSequences);
    void setAudioEffectPluginSequences(
        std::vector<Musec::Audio::Track::PluginSequence<double>>&& audioEffectPluginSequences);
public:
    TrackType trackType() const override;
private:
    std::vector<Musec::Audio::Track::PluginSequence<double>> midiEffectPluginSequences_;
    std::shared_ptr<Musec::Audio::Plugin::IPlugin<double>> instrument_;
    std::vector<Musec::Audio::Track::PluginSequence<double>> audioEffectPluginSequences_;
};
}
}
}

#endif //MUSEC_AUDIO_TRACK_INSTRUMENTTRACK
