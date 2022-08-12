#include "MIDISequence.hpp"

#include <algorithm>

namespace Musec::Audio::Media
{
std::size_t MIDISequence::noteCount() const noexcept
{
    return notes_.size();
}

std::size_t MIDISequence::automationCount() const noexcept
{
    return 0;
}

template<MIDISequence::CheckBound C> const Note& MIDISequence::noteAt(std::size_t index) const
{
    if constexpr(C == CheckBound::kCheck)
    {
        return notes_.at(index);
    }
    else
    {
        return notes_[index];
    }
}

template<MIDISequence::CheckBound C> Note& MIDISequence::noteAt(std::size_t index)
{
    return const_cast<Note&>(static_cast<const MIDISequence&>(*this).noteAt<C>(index));
}

template<MIDISequence::CheckBound C> const MIDISequence::AutomationType&
MIDISequence::automationAt(std::size_t index) const
{
    if constexpr(C == CheckBound::kCheck)
    {
        return automations_.at(index);
    }
    else
    {
        return automations_[index];
    }
}

template<MIDISequence::CheckBound C> MIDISequence::AutomationType& MIDISequence::automationAt(std::size_t index)
{
    return const_cast<AutomationType&>(static_cast<const MIDISequence&>(*this).automationAt<C>(index));
}

std::uint8_t MIDISequence::highestNote() const
{
    auto it = std::max_element(notes_.begin(), notes_.end(),
        [](const Note& lhs, const Note& rhs)
        {
            return lhs.midi < rhs.midi;
        }
    );
    return it != notes_.end()? it->midi: 128;
}

std::uint8_t MIDISequence::lowestNote() const
{
    if(notes_.empty())
    {
        return 128;
    }
    auto it = std::min_element(notes_.begin(), notes_.end(),
        [](const Note& lhs, const Note& rhs)
        {
            return lhs.midi < rhs.midi;
        }
    );
    return it != notes_.end()? it->midi: 128;
}
}