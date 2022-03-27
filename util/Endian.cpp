#include "Endian.hpp"

#include <algorithm>
#include <cstdint>

namespace Musec::Util
{
constexpr Endian endian()
{
    // FIXME: 跨平台时的字节序判断
    return Endian::LittleEndian;
}

void reverseEndianness(void* data, std::size_t byteCount)
{
    std::reverse(reinterpret_cast<std::byte*>(data), reinterpret_cast<std::byte*>(data) + byteCount);
}

void reverseEndiannessCopy(void* src, std::size_t byteCount, void* dest)
{
    std::reverse_copy(reinterpret_cast<std::byte*>(src), reinterpret_cast<std::byte*>(src) + byteCount, dest);
}
}