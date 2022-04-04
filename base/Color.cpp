#include "Color.hpp"

#include <system_error>

namespace Musec::Base
{
Color discardAlpha(const ColorWithAlpha& colorWithAlpha)
{
    return {colorWithAlpha.r, colorWithAlpha.g, colorWithAlpha.b};
}

std::array<std::uint8_t, 3> bgr(const Color& color)
{
    return {color.b, color.g, color.r};
}

ColorStringWithoutHashtag rgbToCharWithoutHashtag(const Color& color, bool uppercase)
{
    std::array<char, 7> ret = {0};
    std::error_code ec;
    std::to_chars(ret.data(), ret.data() + 2, color.r, 16);
    std::to_chars(ret.data() + 2, ret.data() + 4, color.g, 16);
    std::to_chars(ret.data() + 4, ret.data() + 6, color.b, 16);
    if(uppercase)
    {
        for(auto& c: ret)
        {
            if(c >= 'a' && c <= 'z')
            {
                c -= 'a' - 'A';
            }
        }
    }
    return ret;
}

ColorStringWithHashtag rgbToCharWithHashtag(const Color& color, bool uppercase)
{
    std::array<char, 8> ret = {0};
    ret[0] = Hashtag;
    std::error_code ec;
    std::to_chars(ret.data() + 1, ret.data() + 3, color.r, 16);
    std::to_chars(ret.data() + 3, ret.data() + 5, color.g, 16);
    std::to_chars(ret.data() + 5, ret.data() + 7, color.b, 16);
    if(uppercase)
    {
        for(auto& c: ret)
        {
            if(c >= 'a' && c <= 'z')
            {
                c -= 'a' - 'A';
            }
        }
    }
    return ret;
}
}