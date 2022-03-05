#ifndef MUSEC_AUDIO_UTIL
#define MUSEC_AUDIO_UTIL

#include <cstdint>

namespace Musec
{
namespace Audio
{
namespace Util
{
constexpr double A4Frequency = 440.0;

double noteToFrequency(std::uint8_t note, double A4Freq = A4Frequency);

double frequencyToNote(double frequency, double A4Freq = A4Frequency);

double scaleToDecibel(double scale);

float scaleToDecibel(float scale);

long double scaleToDecibel(long double scale);

double decibelToScale(double decibel);

float decibelToScale(float decibel);

long double decibelToScale(long double decibel);
}
}
}

#endif //MUSEC_AUDIO_UTIL
