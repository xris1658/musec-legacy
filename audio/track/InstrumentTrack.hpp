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
    ~InstrumentTrack() override;
public:
    const std::vector<Musec::Audio::Track::PluginSequence>& getMIDIEffectPluginSequences() const;
    void setMIDIEffectPluginSequences(
        const std::vector<Musec::Audio::Track::PluginSequence>& midiEffectPluginSequences);
    void setMIDIEffectPluginSequences(
        std::vector<Musec::Audio::Track::PluginSequence>&& midiEffectPluginSequences);
    const std::shared_ptr<Musec::Audio::Plugin::IPlugin>& getInstrument() const;
    void setInstrument(const std::shared_ptr<Musec::Audio::Plugin::IPlugin>& instrument);
    void setInstrument(std::shared_ptr<Musec::Audio::Plugin::IPlugin>&& instrument);
    const std::vector<Musec::Audio::Track::PluginSequence>& getAudioEffectPluginSequences() const;
    void setAudioEffectPluginSequences(
        const std::vector<Musec::Audio::Track::PluginSequence>& audioEffectPluginSequences);
    void setAudioEffectPluginSequences(
        std::vector<Musec::Audio::Track::PluginSequence>&& audioEffectPluginSequences);
public:
    TrackType trackType() const override;
private:
    std::vector<Musec::Audio::Track::PluginSequence> midiEffectPluginSequences_;
    std::shared_ptr<Musec::Audio::Plugin::IPlugin> instrument_;
    std::vector<Musec::Audio::Track::PluginSequence> audioEffectPluginSequences_;
};
}
}
}

#endif //MUSEC_AUDIO_TRACK_INSTRUMENTTRACK
