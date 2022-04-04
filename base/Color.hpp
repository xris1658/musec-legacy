#ifndef MUSEC_BASE_COLOR
#define MUSEC_BASE_COLOR

#include <array>
#include <charconv>

namespace Musec
{
namespace Base
{
constexpr char Hashtag = '#';

using ColorStringWithoutHashtag = std::array<char, 7>;
using ColorStringWithHashtag = std::array<char, 8>;
struct Color
{
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
};

struct ColorWithAlpha
{
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a;
};

Color discardAlpha(const ColorWithAlpha& colorWithAlpha);

std::array<std::uint8_t, 3> bgr(const Color& color);

ColorStringWithoutHashtag rgbToCharWithoutHashtag(const Color& color, bool uppercase = false);

ColorStringWithHashtag rgbToCharWithHashtag(const Color& color, bool uppercase = false);
}
}

#endif //MUSEC_BASE_COLOR
