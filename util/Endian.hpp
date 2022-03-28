#ifndef MUSEC_UTIL_ENDIAN
#define MUSEC_UTIL_ENDIAN

#include <cstddef>

namespace Musec
{
namespace Util
{
enum class Endian
{
    LittleEndian,
    BigEndian
};

constexpr Endian endian();

void reverseEndianness(void* data, std::size_t byteCount);

void reverseEndiannessCopy(void* src, std::size_t byteCount, void* dest);
}
}

#endif //MUSEC_UTIL_ENDIAN
