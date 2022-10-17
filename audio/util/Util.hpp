#ifndef MUSEC_AUDIO_UTIL
#define MUSEC_AUDIO_UTIL

#include <cstdint>
#include "math/Constants.hpp"

namespace Musec
{
namespace Audio
{
namespace Util
{
constexpr double A4Frequency = 440.0;

enum class PanLaw
{
    k0,
    kN3,
    kN3Comp,
    kN6
};

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
    if constexpr(PL == PanLaw::k0)
    {
        return panning < 0?
            StereoChannelScaleCollection<T>{T(1), T(1) + panning}:
            StereoChannelScaleCollection<T>{T(1) - panning, T(1)};
    }
    else if constexpr(PL == PanLaw::kN3)
    {
        T left = std::cos(T(0.5) * Musec::Math::Pi<T> * (panning * 0.5 + 0.5)) * Musec::Math::Sqrt2<T>;
        return {left, 1 - left * left};
    }
    else if constexpr(PL == PanLaw::kN3Comp)
    {
        auto ret = fromPanning<T, PanLaw::kN3>(panning);
        ret.left *= Musec::Math::Sqrt2<T>;
        ret.right *= Musec::Math::Sqrt2<T>;
        return ret;
    }
    else if constexpr(PL == PanLaw::kN6)
    {
        return {T(-0.5) * panning + T(0.5), T(0.5) * panning + T(0.5)};
    }
}
}
}
}

#endif //MUSEC_AUDIO_UTIL
