#ifndef MUSEC_AUDIO_BASE_TIMESIGNATURE
#define MUSEC_AUDIO_BASE_TIMESIGNATURE

#include <array>
#include <cstdint>
#include <stdexcept>

namespace Musec
{
namespace Audio
{
namespace Base
{
class TimeSignature
{
public:
    TimeSignature(uint8_t numerator = 4, uint8_t denominator = 4);
    TimeSignature(const TimeSignature&);
    TimeSignature& operator=(const TimeSignature&);
    TimeSignature(TimeSignature&&) noexcept;
    TimeSignature& operator=(TimeSignature&&) noexcept;
    ~TimeSignature() noexcept;
public:
    static bool verifyNumerator(std::uint8_t numerator);
    static bool verifyDenominator(std::uint8_t denominator);
public:
    std::uint8_t getNumerator() const;
    void setNumerator(uint8_t numerator);
    std::uint8_t getDenominator() const;
    void setDenominator(uint8_t denominator);
private:
    std::uint8_t numerator_;
    std::uint8_t denominator_;
};
}
}
}

#endif //MUSEC_AUDIO_BASE_TIMESIGNATURE
