#ifndef MUSEC_DKWTP_TIMESIGNATURESEQUENCE
#define MUSEC_DKWTP_TIMESIGNATURESEQUENCE

#include "TimeSignature.hpp"

#include "dkwtp/Chrono.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace Musec
{
namespace DKWTP
{
template<std::size_t PPQ>
struct TimeSignatureChange
{
    Musec::DKWTP::TimePoint<PPQ> timePoint_;
    TimeSignature timeSignature_;
};

struct TimePointLiteral
{
    std::int64_t bar_;
    std::uint8_t beat_;
    std::uint16_t pulse_;
};

template<std::size_t PPQ>
class TimeSignatureSequence
{
private:
    using TimeSignatureChangeVector = std::vector<TimeSignatureChange<PPQ>>;
    using TimeSignatureChangeVectorIterator = typename TimeSignatureChangeVector::iterator;
    using TimeSignatureChangeVectorConstIterator = typename TimeSignatureChangeVector::const_iterator;
    using TimeSignatureChangeVectorReverseIterator = typename TimeSignatureChangeVector::reverse_iterator;
    using TimeSignatureChangeVectorConstReverseIterator = typename TimeSignatureChangeVector::const_reverse_iterator;
public:
    explicit TimeSignatureSequence(TimeSignature initialTimeSignature):
        timeSignatureChange_()
    {
        timeSignatureChange_.emplace_back(TimePoint<PPQ>(0), initialTimeSignature);
    }
    TimeSignatureSequence(const TimeSignatureSequence<PPQ>&) = default;
    TimeSignatureSequence<PPQ>& operator=(const TimeSignatureSequence<PPQ>&) = default;
    TimeSignatureSequence(TimeSignatureSequence<PPQ>&&) noexcept = default;
    TimeSignatureSequence<PPQ>& operator=(TimeSignatureSequence<PPQ>&&) noexcept = default;
    ~TimeSignatureSequence() noexcept = default;
public:
    static bool timeSignatureChangeTimeLess(const TimeSignatureChange<PPQ>& lhs, const TimePoint<PPQ>& rhs)
    {
        return lhs.timePoint_ < rhs;
    }
public:
    bool empty() const
    {
        return timeSignatureChange_.empty();
    }
    std::size_t size() const
    {
        return timeSignatureChange_.size();
    }
private:
    TimeSignatureChangeVectorIterator lowerBound(const TimePoint<PPQ>& timePoint)
    {
        return std::lower_bound(timeSignatureChange_.begin(),
                                timeSignatureChange_.end(),
                                timePoint,
                                timeSignatureChangeTimeLess);
    }
    TimeSignatureChangeVectorConstIterator lowerBound(const TimePoint<PPQ>& timePoint) const
    {
        return std::lower_bound(timeSignatureChange_.cbegin(),
                                timeSignatureChange_.cend(),
                                timePoint,
                                timeSignatureChangeTimeLess);
    }
public:
    const TimeSignatureChange<PPQ>& operator[](std::size_t index) const
    {
        return timeSignatureChange_[index];
    }
    TimeSignatureChange<PPQ>& operator[](std::size_t index)
    {
        return const_cast<TimeSignatureChange<PPQ>&>(
            static_cast<const TimeSignatureSequence<PPQ>&>(*this)[index]
        );
    }
    // 获取特定时间的节拍
    TimeSignature operator()(const TimePoint<PPQ>& timePoint) const
    {
        if(timePoint.pulse() == 0)
        {
            return timeSignatureChange_.begin()->timeSignature_;
        }
        auto notBeforeTimePoint = lowerBound(timePoint);
        if(notBeforeTimePoint == timeSignatureChange_.end())
        {
            return timeSignatureChange_.rbegin()->timeSignature_;
        }
        if(notBeforeTimePoint->timePoint_ == timePoint)
        {
            return notBeforeTimePoint->timeSignature_;
        }
        auto beforeTimePoint = notBeforeTimePoint - 1;
        return beforeTimePoint->timeSignature_;
    }
private:
    TimeSignatureChangeVector timeSignatureChange_;
};
}
}

#endif //MUSEC_DKWTP_TIMESIGNATURESEQUENCE
