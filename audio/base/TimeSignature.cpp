#include "TimeSignature.hpp"

#include <charconv>

namespace Musec::Audio::Base
{
TimeSignature::TimeSignature(uint8_t numerator, uint8_t denominator)
{
    if((!verifyNumerator(numerator_)) || (!verifyDenominator(denominator_)))
    {
        throw std::invalid_argument("Error: Unqualified numerator or denominator.");
    }
    numerator_ = numerator;
    denominator_ = denominator;
}
TimeSignature::TimeSignature(const TimeSignature&) = default;
TimeSignature& TimeSignature::operator=(const TimeSignature&) = default;
TimeSignature::TimeSignature(TimeSignature&&) noexcept = default;
TimeSignature& TimeSignature::operator=(TimeSignature&&) noexcept = default;
TimeSignature::~TimeSignature() noexcept = default;

// 1 - 32
bool TimeSignature::verifyNumerator(std::uint8_t numerator)
{
    return (numerator != 0) && numerator <= 32;
}

// 2, 4, 8, 16, 32, 64
bool TimeSignature::verifyDenominator(std::uint8_t denominator)
{
    return (((denominator & 0x02) != 0)
          + ((denominator & 0x04) != 0)
          + ((denominator & 0x08) != 0)
          + ((denominator & 0x10) != 0)
          + ((denominator & 0x20) != 0)
          + ((denominator & 0x40) != 0)
        == 1)
    &&     (((denominator & 0x01) != 0)
          + ((denominator & 0x80) != 0)
        == 0);
}
uint8_t TimeSignature::getNumerator() const
{
    return numerator_;
}
void TimeSignature::setNumerator(uint8_t numerator)
{
    if (!verifyNumerator(numerator))
    {
        throw std::invalid_argument("Error: Unqualified numerator.");
    }
    numerator_ = numerator;
}
uint8_t TimeSignature::getDenominator() const
{
    return denominator_;
}
void TimeSignature::setDenominator(uint8_t denominator)
{
    if(!verifyDenominator(denominator))
    {
        throw std::invalid_argument("Error: Unqualified denominator.");
    }
    denominator_ = denominator;
}

}