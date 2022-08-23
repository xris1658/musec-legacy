#ifndef MUSEC_BASE_BASE
#define MUSEC_BASE_BASE

#include <cstdlib>
#include <cstring>
#include <type_traits>
#include <tuple>

namespace Musec
{
namespace Base
{
template<typename T, std::size_t N>
constexpr std::size_t stackArraySize(const T(&)[N]) noexcept
{
    return N;
}

// std::memset
template<typename T>
void memoryReset(T* const address)
{
    std::memset(address, 0, sizeof(T));
}

template<typename T>
auto underlyingValue(T enumerate)
{
    static_assert(std::is_enum_v<T>);
    return static_cast<std::underlying_type_t<T>>(enumerate);
}
}
}

#endif // MUSEC_BASE_BASE
