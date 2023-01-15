#include "MIDIMessageCast.hpp"

#include "base/Base.hpp"

#include <winrt/Windows.Devices.Midi.h>

namespace Musec::Audio::MIDI
{
// VST3
// See
// - https://steinbergmedia.github.io/vst3_dev_portal/pages/Technical+Documentation/About+MIDI/Index.html
//   or vst3_dev_portal/book/html/pages/Technical+Documentation/About+MIDI/Index.html
template<> auto messageCast<Steinberg::Vst::NoteOnEvent>(const MIDIMessage& from)
{
    Steinberg::Vst::NoteOnEvent ret {};
    const auto& noteOnMessage = from.message()->message()->as<MidiNoteOnMessage>();
    ret.channel = noteOnMessage.Channel();
    ret.pitch = noteOnMessage.Note();
    ret.tuning = 0;
    ret.velocity = static_cast<float>(noteOnMessage.Velocity() * 127);
    ret.noteId = -1;
    return ret;
}

template<> auto messageCast<Steinberg::Vst::NoteOffEvent>(const MIDIMessage& from)
{
    Steinberg::Vst::NoteOffEvent ret {};
    const auto& noteOffMessage = from.message()->message()->as<MidiNoteOffMessage>();
    ret.channel = noteOffMessage.Channel();
    ret.pitch = noteOffMessage.Note();
    ret.velocity = static_cast<float>(noteOffMessage.Velocity() * 127);
    ret.noteId = -1;
    ret.tuning = 0;
    return ret;
}

// VST3 - Only System Exclusive message uses this
template<> auto messageCast<Steinberg::Vst::DataEvent>(const MIDIMessage& from)
{
    Steinberg::Vst::DataEvent ret {};
    ret.size = from.rawDataSize();
    ret.type = Steinberg::Vst::DataEvent::kMidiSysEx;
    ret.bytes = reinterpret_cast<const Steinberg::uint8*>(from.rawData());
    return ret;
}

// CLAP
// See
// - clap/include/clap/events.h
template<> auto messageCast<clap_event_note>(const MIDIMessage& from)
{
    constexpr auto increment8Bit = 1.0 / 127;
    clap_event_note ret {};
    ret.header.size = sizeof(clap_event_note);
    ret.header.time = 0; // FIXME
    ret.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    auto type = from.message()->message()->Type();
    if(type == MidiMessageType::NoteOn)
    {
        ret.header.type = CLAP_EVENT_NOTE_ON;
        const auto& noteOnMessage = from.message()->message()->as<MidiNoteOnMessage>();
        ret.port_index = -1; // FIXME: CLAP note port (see clap/include/clap/ext/note-ports.h)
        ret.channel = noteOnMessage.Channel();
        ret.key = noteOnMessage.Note();
        ret.velocity = noteOnMessage.Velocity() * increment8Bit;
    }
    else if(type == MidiMessageType::NoteOff)
    {
        ret.header.type = CLAP_EVENT_NOTE_OFF;
        const auto& noteOffMessage = from.message()->message()->as<MidiNoteOffMessage>();
        ret.port_index = -1; // FIXME: CLAP note port (see clap/include/clap/ext/note-ports.h)
        ret.channel = noteOffMessage.Channel();
        ret.key = noteOffMessage.Note();
        ret.velocity = noteOffMessage.Velocity() * increment8Bit;
    }
    ret.header.flags = 0;
    ret.note_id = -1;
    return ret;
}

// CLAP - MIDI events other than note on / note off
template<> auto messageCast<clap_event_midi>(const MIDIMessage& from)
{
    clap_event_midi ret {};
    ret.header.size = sizeof(clap_event_note);
    ret.header.time = 0; // FIXME
    ret.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    ret.header.type = CLAP_EVENT_MIDI;
    ret.header.flags = 0;
    ret.port_index = -1; // FIXME: CLAP note port (see clap/include/clap/ext/note-ports.h)
    assert(from.rawDataSize() <= Musec::Base::stackArraySize(ret.data));
    std::memcpy(ret.data, from.rawData(), from.rawDataSize());
    return ret;
}

template<> auto messageCast<clap_event_midi_sysex>(const MIDIMessage& from)
{
    clap_event_midi_sysex ret {};
    ret.header.size = sizeof(clap_event_midi_sysex);
    ret.header.time = 0; // FIXME
    ret.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
    ret.header.type = CLAP_EVENT_MIDI_SYSEX;
    ret.header.flags = 0;
    ret.port_index = -1; // FIXME: CLAP note port (see clap/include/clap/ext/note-ports.h)
    ret.buffer = reinterpret_cast<const uint8_t*>(from.rawData());
    ret.size = from.rawDataSize();
    return ret;
}
}