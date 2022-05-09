#ifndef MUSEC_CONSTANTS_HPP
#define MUSEC_CONSTANTS_HPP

#include <cstddef>

namespace Musec
{
namespace Audio
{
namespace Base
{
template<std::size_t Bit>
constexpr float leftShift()
{
    return 1ll << Bit;
}
constexpr float Int8Max  = leftShift<7>();
constexpr float Int16Max = leftShift<15>();
constexpr float Int18Max = leftShift<17>();
constexpr float Int20Max = leftShift<19>();
constexpr float Int24Max = leftShift<23>();
constexpr float Int32Max = leftShift<31>();
constexpr float Int64Max = leftShift<63>();
}
}
}

#endif //MUSEC_CONSTANTS_HPP
