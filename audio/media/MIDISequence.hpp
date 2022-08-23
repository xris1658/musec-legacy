#ifndef MUSEC_AUDIO_MEDIA_MIDISEQUENCE
#define MUSEC_AUDIO_MEDIA_MIDISEQUENCE

#include "audio/base/Automation.hpp"

#include <array>
#include <set>
#include <vector>

namespace Musec
{
namespace Audio
{
namespace Media
{
using Musec::Audio::Base::Automation;
struct Note
{
    std::int64_t startTime;
    std::int64_t duration;
    std::uint8_t midi;
    // TODO: Add MIDI polyphonic expression (MPE) support.
    // To enable MPE support of VST3 plugins, the corresponding *.obj file is needed.
};

class MIDISequence
{
public:
    // Store value as `double` to deal with dynamic precision.
    using AutomationType = Automation;
    static_assert(std::is_same_v<AutomationType::ValueType, double>);
public:
    enum CheckBound
    {
        kCheck,
        kNotCheck
    };
public:
    std::size_t noteCount() const noexcept;
    std::size_t automationCount() const noexcept;
    template<CheckBound C = CheckBound::kCheck>
    const Note& noteAt(std::size_t index) const;
    template<CheckBound C>
    Note& noteAt(std::size_t index);
    template<CheckBound C>
    const AutomationType& automationAt(std::size_t index) const;
    template<CheckBound C>
    AutomationType& automationAt(std::size_t index);
public:
    std::uint8_t highestNote() const;
    std::uint8_t lowestNote() const;
private:
    std::vector<Note> notes_;
    std::array<AutomationType, 128> automations_;
};
}
}
}

#endif //MUSEC_AUDIO_MEDIA_MIDISEQUENCE
