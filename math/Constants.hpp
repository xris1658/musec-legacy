#ifndef MUSEC_MATH_CONSTANTS
#define MUSEC_MATH_CONSTANTS

#include <cmath>

#if __cplusplus >= 202002L
#include <numbers>
#endif

namespace Musec
{
namespace Math
{
#if __cplusplus >= 202002L
template<typename T>
constexpr T Pi = std::numbers::pi_v<T>;

template<typename T>
constexpr T Sqrt2 = std::numbers::sqrt2_v<T>;
#else
template<typename T>
constexpr T Pi = std::acos(T(-1));

template<typename T>
constexpr T Sqrt2 = std::sqrt(T(2));
#endif
}
}

#endif //MUSEC_MATH_CONSTANTS
