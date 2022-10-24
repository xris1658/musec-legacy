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
enum PanLaw
{
    k0,
    kN3,
    kN6
};

constexpr PanLaw DefaultPanLaw = PanLaw::kN3;

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

template<typename T, PanLaw panLaw>
StereoChannelScaleCollection<T> fromPanning(T panning)
{
    static_assert(std::is_floating_point_v<T>);
    if constexpr(panLaw == PanLaw::k0)
    {
        return panning < 0?
            StereoChannelScaleCollection<T>{T(1), T(1) + panning}:
            StereoChannelScaleCollection<T>{T(1) - panning, T(1)};
    }
    else if constexpr(panLaw == PanLaw::kN3)
    {
        T left = std::cos(T(0.25) * Musec::Math::pi<T>() * (panning + 1));
        return {left, std::sqrt(1 - left * left)};
    }
    else if constexpr(panLaw == PanLaw::kN6)
    {
        return {T(0.5) - T(0.5) * panning, T(0.5) + T(0.5) * panning};
    }
    return {1, 1};
}

template<typename T, PanLaw panLaw>
T compensateFromPanLaw()
{
    static_assert(std::is_floating_point_v<T>);
    if constexpr(panLaw == PanLaw::k0)
    {
        return 1;
    }
    else if constexpr(panLaw == PanLaw::kN3)
    {
        return Musec::Math::sqrt2<T>();
    }
    else if constexpr(panLaw == PanLaw::kN6)
    {
        return 2;
    }
}
}
}
}

#endif //MUSEC_AUDIO_UTIL
