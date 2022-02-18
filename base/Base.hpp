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
// 获取栈上原始数组的元素个数
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
}
}

#endif // MUSEC_BASE_BASE
