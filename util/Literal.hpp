#ifndef MUSEC_AUDIO_UTIL_LITERAL
#define MUSEC_AUDIO_UTIL_LITERAL

#include <array>

namespace Musec
{
namespace Util
{
enum class MIDINoteOffset
{
    Yamaha = -2,
    Roland = -1,
    NoOffset = 0
};

enum class MIDINoteNameKeyShift
{
    Sharp,
    Flat
};

template<typename CharType>
using NoteName = std::array<CharType, 5>;

template<typename CharType>
NoteName<CharType> getNoteName(std::int8_t note, MIDINoteNameKeyShift keyShift, MIDINoteOffset offset = MIDINoteOffset::Yamaha);

extern template NoteName<char> getNoteName(std::int8_t, MIDINoteNameKeyShift, MIDINoteOffset);
extern template NoteName<wchar_t> getNoteName(std::int8_t, MIDINoteNameKeyShift, MIDINoteOffset);
extern template NoteName<char16_t> getNoteName(std::int8_t, MIDINoteNameKeyShift, MIDINoteOffset);
extern template NoteName<char32_t> getNoteName(std::int8_t, MIDINoteNameKeyShift, MIDINoteOffset);
#if __cplusplus >= 202002L
extern template NoteName<char8_t> getNoteName(std::int8_t, MIDINoteNameKeyShift, MIDINoteOffset);
#endif
}
}

#endif //MUSEC_AUDIO_UTIL_LITERAL
