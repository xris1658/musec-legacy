#ifndef MUSEC_AUDIO_UTIL
#define MUSEC_AUDIO_UTIL

#include "math/Constants.hpp"

#include <cstdint>

namespace Musec
{
namespace Audio
{
namespace Util
{
enum class PanLaw
{
    k0,
    kN3,
    kN6
};

constexpr double A4Frequency = 440.0;

template<typename T>
struct StereoChannelScaleCollection
{
    float left;
    float right;
};

double noteToFrequency(std::uint8_t note, double A4Freq = A4Frequency);

double frequencyToNote(double frequency, double A4Freq = A4Frequency);

double scaleToDecibel(double scale);

float scaleToDecibel(float scale);

long double scaleToDecibel(long double scale);

double decibelToScale(double decibel);

float decibelToScale(float decibel);

long double decibelToScale(long double decibel);

template<typename T, PanLaw PL>
StereoChannelScaleCollection<T> fromPanning(T panning)
{
    static_assert(std::is_floating_point_v<T>);
    if constexpr(PL == PanLaw::k0)
    {
        return panning < 0?
            StereoChannelScaleCollection<T>{T(1), T(1) + panning}:
            StereoChannelScaleCollection<T>{T(1) - panning, T(1)};
    }
    else if constexpr(PL == PanLaw::kN3)
    {
        T left = std::cos(T(0.5) * Musec::Math::Pi<T> * (panning * 0.5 + 0.5)) * Musec::Math::Sqrt2<T>;
        return {left, std::sqrt(1 - left * left)};
    }
    else if constexpr(PL == PanLaw::kN6)
    {
        return {T(0.5) - T(0.5) * panning, T(0.5) + T(0.5) * panning};
    }
}
}
}
}

#endif //MUSEC_AUDIO_UTIL
