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
constexpr T pi()
{
    return std::numbers::pi_v<T>;
}

template<typename T>
constexpr T sqrt2()
{
    return std::numbers::sqrt2_v<T>;
}
#else

// Variable template and `constexpr` is not used for several reasons:
// - Functions in <cmath> might modify global variables like `errno`
// - Some functions in <cmath> depend on the rounding mode at runtime
template<typename T>
T pi()
{
    return std::acos(-1.0l);
}

template<typename T>
T sqrt2()
{
    return std::sqrt(2.0l);
}
#endif
}
}

#endif //MUSEC_MATH_CONSTANTS
