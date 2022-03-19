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

// 将限域枚举转换为其底层类型
template<typename T>
auto underlyingValue(T enumerate)
{
    static_assert(std::is_enum_v<T>);
    return static_cast<std::underlying_type_t<T>>(enumerate);
}
}
}

#endif // MUSEC_BASE_BASE
