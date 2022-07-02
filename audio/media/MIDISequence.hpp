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
    // TODO: 添加关于 MIDI 复音表情（MPE）的内容，
    // 要启用 VST3 的 MPE 支持，需要添加 VST3 相关的 obj 文件。
};

class MIDISequence
{
public:
    // 将值存储为 double 格式以应对动态精度
    using AutomationType = Automation<std::int8_t, double>;
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
    // 所有音符
    std::vector<Note> notes_;
    std::array<AutomationType, 128> automations_;
};
}
}
}

#endif //MUSEC_AUDIO_MEDIA_MIDISEQUENCE
