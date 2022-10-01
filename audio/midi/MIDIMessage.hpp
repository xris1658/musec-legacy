#ifndef MUSEC_AUDIO_MIDI_MIDIMESSAGE
#define MUSEC_AUDIO_MIDI_MIDIMESSAGE

#include <winrt/Windows.Devices.Midi.h>

#include <chrono>

namespace Musec
{
namespace Native
{
class MIDIMessageImpl;
}
}

namespace Musec
{
namespace Audio
{
namespace MIDI
{
using SteadyClockDuration = decltype(std::chrono::steady_clock::now().time_since_epoch());

// MIDI message type
// - translate to VST2 MIDI events
//   - vstsdk2.4/doc/html/struct_vst_midi_event.html
// - translate to VST3 events
//   - https://steinbergmedia.github.io/vst3_dev_portal/pages/Technical+Documentation/About+MIDI/Index.html
// - translate to CLAP events
//   - clap/include/clap/events.h
//   - MIDI notes events are separate in CLAP
// - translate to native type
//   - Event classes in `winrt::Windows::Devices::Midi`
enum MIDIMessageType
{
    UnknownMessage = -1,
    NoneMessage,
    NoteOnMessage,
    NoteOffMessage,
    PolyKeyPressureMessage,
    ControlChangeMessage,
    ChannelPressureMessage,
    PitchBendMessage,
    ProgramChangeMessage,
    SystemExclusiveMessage
};

class MIDIMessage
{
public:
    MIDIMessageType type() const;
    SteadyClockDuration timestamp() const;
    const std::byte* rawData() const;
    std::size_t rawDataSize() const;
    const Musec::Native::MIDIMessageImpl* message() const;
private:
    Musec::Native::MIDIMessageImpl* message_;
};
}
}
}

#endif //MUSEC_AUDIO_MIDI_MIDIMESSAGE
