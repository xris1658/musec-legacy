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

constexpr PanLaw DefaultPanLaw = PanLaw::kN3;

constexpr double A4Frequency = 440.0;

template<typename T>
struct StereoChannelScaleCollection
{
    T left;
    T right;
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
        return panning <= 0?
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

#pragma warning(push)
#pragma warning(disable: 4702) // Unreachable code
template<typename T, PanLaw panLaw, bool Compensate>
StereoChannelScaleCollection<T> fromPanning(T panning)
{
    static_assert(std::is_floating_point_v<T>);
    using namespace Musec::Math;
    if constexpr(panLaw == PanLaw::k0)
    {
        return panning <= 0?
            StereoChannelScaleCollection<T>{T(1), T(1) + panning}:
            StereoChannelScaleCollection<T>{T(1) - panning, T(1)};
    }
    else if constexpr(panLaw == PanLaw::kN3)
    {
        if(panning == 0)
        {
            if constexpr(Compensate)
            {
                return {T(1), T(1)};
            }
            return {sqrt2<T>(), sqrt2<T>()};
        }
        T left = std::cos(T(0.25) * Musec::Math::pi<T>() * (panning + 1));
        if constexpr(Compensate)
        {
            return {left * sqrt2<T>(), std::sqrt((1 - left * left) * 2)};
        }
        return {left, std::sqrt(1 - left * left)};
    }
    else if constexpr(panLaw == PanLaw::kN6)
    {
        if constexpr(Compensate)
        {
            return {T(1) - T(1) * panning, T(1) + T(1) * panning};
        }
        return {T(0.5) - T(0.5) * panning, T(0.5) + T(0.5) * panning};
    }
}
#pragma warning(pop)
}
}
}

#endif //MUSEC_AUDIO_UTIL
