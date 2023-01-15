#ifndef MUSEC_AUDIO_MIDI_MIDIMESSAGECAST
#define MUSEC_AUDIO_MIDI_MIDIMESSAGECAST

#include "MIDIMessage.hpp"

#include "native/MIDIMessageImpl.hpp"

#include <pluginterfaces/vst/ivstevents.h>
#include <clap/events.h>

namespace Musec
{
namespace Audio
{
namespace MIDI
{
using namespace winrt::Windows::Devices::Midi;

template<typename To>
auto messageCast(const MIDIMessage& from);

extern template auto messageCast<Steinberg::Vst::NoteOnEvent>(const MIDIMessage& from);

extern template auto messageCast<Steinberg::Vst::NoteOffEvent>(const MIDIMessage& from);

extern template auto messageCast<Steinberg::Vst::DataEvent>(const MIDIMessage& from);

extern template auto messageCast<clap_event_note>(const MIDIMessage& from);

extern template auto messageCast<clap_event_midi>(const MIDIMessage& from);

extern template auto messageCast<clap_event_midi_sysex>(const MIDIMessage& from);
}
}
}

#endif //MUSEC_AUDIO_MIDI_MIDIMESSAGECAST
