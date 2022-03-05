#include "Util.hpp"

#include <cmath>

namespace Musec::Audio::Util
{
double noteToFrequency(std::uint8_t note, double A4Freq)
{
    return A4Freq * std::pow(2, double(note - uint8_t(57)) / 12.0);
}

double frequencyToNote(double frequency, double A4Freq)
{
    return std::log2(frequency / A4Freq) * 12.0 + 57.0;
}

double scaleToDecibel(double scale)
{
    return 20.0 * std::log10(std::fabs(scale));
}

float scaleToDecibel(float scale)
{
    return 20.0F * std::log10(std::fabs(scale));
}

long double scaleToDecibel(long double scale)
{
    return 20.0L * std::log10(std::fabs(scale));
}

double decibelToScale(double decibel)
{
    return std::pow(10.0, decibel * 0.05);
}

float decibelToScale(float decibel)
{
    return std::pow(10.0F, decibel * 0.05F);
}

long double decibelToScale(long double decibel)
{
    return std::pow(10.0L, decibel * 0.05L);
}
}