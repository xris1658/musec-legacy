#include "ASIOChannelGroup.hpp"

#include <algorithm>

namespace Musec::Audio::Driver
{
ASIOChannelGroup::ASIOChannelGroup()
{
    //
}

ASIOChannelGroup::ASIOChannelGroup(ASIOChannelGroup::Direction direction, long channelCount):
    direction_(direction),
    channelCount_(channelCount),
    channelIndices_(new long[channelCount])
{
    std::fill(channelIndices_, channelIndices_ + channelCount, InvalidChannelIndex);
}

ASIOChannelGroup::ASIOChannelGroup(const ASIOChannelGroup& rhs):
    direction_(rhs.direction_),
    channelCount_(rhs.channelCount_),
    channelIndices_(new long[rhs.channelCount_])
{
    std::copy(rhs.channelIndices_, rhs.channelIndices_ + rhs.channelCount_, channelIndices_);
}

ASIOChannelGroup::ASIOChannelGroup(ASIOChannelGroup&& rhs) noexcept
{
    swap(rhs);
}

ASIOChannelGroup& ASIOChannelGroup::operator=(const ASIOChannelGroup& rhs)
{
    if(&rhs != this)
    {
        *this = ASIOChannelGroup();
        direction_ = rhs.direction_;
        channelCount_ = rhs.channelCount_;
        channelIndices_ = new long[channelCount_];
        std::copy(rhs.channelIndices_, rhs.channelIndices_ + channelCount_, channelIndices_);
    }
    return *this;
}

ASIOChannelGroup& ASIOChannelGroup::operator=(ASIOChannelGroup&& rhs) noexcept
{
    if(&rhs != this)
    {
        swap(rhs);
    }
    return *this;
}

ASIOChannelGroup::~ASIOChannelGroup() noexcept
{
    if(channelIndices_)
    {
        delete[] channelIndices_;
    }
}

ASIOChannelGroup::Direction ASIOChannelGroup::direction() const
{
    return direction_;
}

long ASIOChannelGroup::channelCount() const
{
    return channelCount_;
}

long ASIOChannelGroup::channelIndex(long index) const
{
    return channelIndices_[index];
}

void ASIOChannelGroup::setChannelIndex(long index, long channelIndex)
{
    channelIndices_[index] = channelIndex;
}

void ASIOChannelGroup::swap(ASIOChannelGroup& rhs) noexcept
{
    std::swap(direction_, rhs.direction_);
    std::swap(channelCount_, rhs.channelCount_);
    std::swap(channelIndices_, rhs.channelIndices_);
}
}