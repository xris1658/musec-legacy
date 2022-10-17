#ifndef MUSEC_MATH_CONSTANTS
#define MUSEC_MATH_CONSTANTS

#include <cmath>

namespace Musec
{
namespace Math
{
template<typename T>
constexpr T Pi = std::acos(T(-1));

template<typename T>
constexpr T Sqrt2 = std::sqrt(T(2));
}
}

#endif //MUSEC_MATH_CONSTANTS
