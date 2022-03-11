#ifndef MUSEC_DKWTP_TEMPOAUTOMATION
#define MUSEC_DKWTP_TEMPOAUTOMATION

#include "dkwtp/Automation.hpp"
#include "dkwtp/Chrono.hpp"

#include <cmath>

namespace Musec
{
namespace DKWTP
{
template<std::size_t PPQ>
using TempoAutomationPoint = AutomationPoint<TimePoint<PPQ>, double>;

template<std::size_t PPQ>
class TempoAutomation: public Automation<TimePoint<PPQ>, double>
{
    using Base = Automation<TimePoint<PPQ>, double>;
private:
    static double secondElapsed(const typename Base::Point& from, const typename Base::Point& to)
    {
        if(from.time_ >= to.time_)
        {
            return 0;
        }
        auto bpmFrom = from.value_;
        auto bpmTo = to.value_;
        auto beatElapsed = (to.time_ - from.time_).duration() / PPQ;
        if (bpmTo == bpmFrom)
        {
            return beatElapsed / bpmFrom * 60.0;
        }
        auto invertK = beatElapsed  / (bpmTo - bpmFrom);
        return (std::log(bpmTo) - std::log(bpmFrom)) * invertK * 60.0;
   }
public:
    std::size_t ppq() const
    {
        return PPQ;
    }
    double secondElapsed(const TimePoint<PPQ>& from, const TimePoint<PPQ>& to) const
    {
        // from 在 to 的前面，返回 secondElapsed(to, from) * -1
        if(from > to)
        {
            return secondElapsed(to, from) * -1.0;
        }
        if(from == to)
        {
            return 0;
        }
        double ret = 0;
        auto notBeforeStart = Base::lowerBound(from);
        auto notBeforeEnd = Base::lowerBound(to);
        // --from--to--point (notBeforeStart, notBeforeEnd)--point--
        // from 和 to 之间没有点（from 和 to 可能均在最后一个点之后出现）
        TempoAutomationPoint<PPQ> pointFrom {from, Base::operator()(from)};
        TempoAutomationPoint<PPQ> pointTo {to, Base::operator()(to)};
        if(notBeforeEnd == notBeforeStart)
        {
            return secondElapsed(pointFrom, pointTo);
        }
        // --from--point (notBeforeStart)--to--point (notBeforeEnd)--
        // from 和 to 之间有点
        ret += secondElapsed(pointFrom, *notBeforeStart);
        auto beforeEnd = notBeforeEnd - 1;
        for(auto it = notBeforeStart; it != beforeEnd; ++it)
        {
            ret += secondElapsed(*it, *(it + 1));
        }
        ret += secondElapsed(*beforeEnd, pointTo);
        return ret;
    }
    double secondElapsedFromZero(const TimePoint<PPQ>& to) const
    {
        return secondElapsed(TimePoint<PPQ>(0), to);
    }
};

}
}

#endif //MUSEC_DKWTP_TEMPOAUTOMATION
