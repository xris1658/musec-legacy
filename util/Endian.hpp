#ifndef MUSEC_UTIL_ENDIAN
#define MUSEC_UTIL_ENDIAN

#include <cstddef>

namespace Musec
{
namespace Util
{
enum class Endian
{
    UnknownEndian,
    LittleEndian, // 不以 k 开头，因为和 Steinberg VST3 SDK 中的宏定义撞车了
    BigEndian
};

constexpr Musec::Util::Endian endian();

void reverseEndianness(void* data, std::size_t byteCount);

void reverseEndiannessCopy(void* src, std::size_t byteCount, void* dest);
}
}

#endif //MUSEC_UTIL_ENDIAN
