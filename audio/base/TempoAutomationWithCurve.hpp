#ifndef MUSEC_DKWTP_TEMPOAUTOMATIONWITHCURVE
#define MUSEC_DKWTP_TEMPOAUTOMATIONWITHCURVE

#include "AutomationWithCurve.hpp"
#include "Chrono.hpp"
#include "math/Integration.hpp"
#include "math/QuadraticFunction.hpp"

#include <cmath>
#include <stdexcept>

namespace Musec
{
namespace Audio
{
namespace Base
{
constexpr auto minimumTempo = 30.0;
constexpr auto maximumTempo = 300.0;

template<std::size_t PPQ>
using TempoAutomationPointWithCurve = AutomationPointWithCurve<TimePoint<PPQ>, double>;

template<std::size_t PPQ>
class TempoAutomationWithCurve: public AutomationWithCurve<TimePoint<PPQ>, double>
{
    using Base = AutomationWithCurve<TimePoint<PPQ>, double>;
private:
    double secondElapsed(double bpm, const Duration<PPQ>& duration) const
    {
        return duration / (bpm * PPQ) * 60.0;
    }
public:
    std::size_t insertPoint(const typename Base::Point& point, std::size_t indexInEqualTimePoint = 0) override
    {
        auto insertedPoint = point;
        if(insertedPoint.value_ < minimumTempo)
        {
            insertedPoint.value_ = minimumTempo;
        }
        else if(insertedPoint.value_ > maximumTempo)
        {
            insertedPoint.value_ = maximumTempo;
        }
        return Base::insertPoint(insertedPoint, indexInEqualTimePoint);
    }
    void deletePoint(std::size_t index) override
    {
        Base::deletePoint(index);
        auto right = Base::begin() + index;
        right->curve_ = 0;
    }
private:
    double secondElapsed(typename Base::PointVectorConstIterator left, const TimePoint<PPQ>& from, const TimePoint<PPQ>& to) const
    {
        auto function = Base::getFunction(left);
        function.a *= PPQ;
        function.b *= PPQ;
        function.c *= PPQ;
        double ret = Musec::Math::quadraticFunctionInvertIntegration(
            function, from, to
        ) * 60.0;
        return ret;
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
        // --from--to--point--
        if (notBeforeStart == notBeforeEnd)
        {
            if (notBeforeStart == Base::cbegin())
            {
                ret = secondElapsed(notBeforeStart->value_, to - from);
                return ret;
            }
            else
            {
                auto beforeStart = notBeforeStart - 1;
                ret = secondElapsed(notBeforeStart, from, to);
                return ret;
            }
        }
        auto afterStart = Base::upperBound(from);
        auto afterEnd = Base::upperBound(to);
        if (afterStart == Base::cend() /*&& afterEnd == Bae::cend()*/)
        {
            ret = secondElapsed((Base::cend() - 1)->value_, to - from);
            return ret;
        }
        // --from--point--point--to--
        // Step 1 of 3: from--
        if(notBeforeStart == Base::cbegin())
        {
            ret += secondElapsed(notBeforeStart->value_, notBeforeStart->time_ - from);
        }
        else
        {
            ret += secondElapsed(notBeforeStart - 1, from, notBeforeStart->time_);
        }
        auto beforeEnd = notBeforeEnd - 1;
        // Step 2 of 3: --point--point--
        for(auto it = notBeforeStart; it < beforeEnd; ++it)
        {
            ret += secondElapsed(it, it->time_, (it + 1)->time_);
        }
        // Step 3 of 3: --to
        if(notBeforeEnd == Base::cend())
        {
            ret += secondElapsed(beforeEnd->value_, to - beforeEnd->time_);
        }
        else
        {
            ret += secondElapsed(beforeEnd, beforeEnd->time_, to);
        }
        return ret;
    }
    double secondElapsedFromZero(const TimePoint<PPQ>& to) const
    {
        return secondElapsed(TimePoint<PPQ>(0), to);
    }
    double secondElapsedInPulse(const TimePoint<PPQ>& pulse) const
    {
        return secondElapsed(pulse, TimePoint<PPQ>(pulse.pulse() + 1));
    }
    Duration<PPQ> pulseElapsedFrom(const TimePoint<PPQ>& from, double second, double precision = 60.0 / Musec::Audio::Base::maximumTempo / static_cast<double>(PPQ)) const
    {
        if(second == 0)
        {
            return Duration<PPQ>(0);
        }
        // from 右侧的点，或者与 from 重合的点
        auto lowerBound = Base::lowerBound(from);
        double sec = from == lowerBound->time_? 0: secondElapsed(from, lowerBound->time_);
        for(auto i = lowerBound; i != Base::cend() - 1; ++i)
        {
            auto durationFromThisToNext = secondElapsed(i, i->time_, (i + 1)->time_);
            if(sec + durationFromThisToNext < second)
            {
                sec += durationFromThisToNext;
            }
            else if(sec + durationFromThisToNext == second)
            {
                return Duration<PPQ>((i + 1)->time_);
            }
            else
            {
                if(i->value_ == (i + 1)->value_)
                {
                    auto pulsePerSecond = i->value_ * PPQ / 60.0;
                    return Duration<PPQ>(std::round(sec + pulsePerSecond * (second - sec)));
                }
                auto remain = second - sec;
                auto ret = i->time_;
                constexpr double half = 0.5;
                double ratio = half;
                auto right = i->time_ + Duration<PPQ>(((i + 1)->time_ - i->time_) * ratio);
                while(true)
                {
                    auto secondEl = secondElapsed(i, i->time_, right);
                    ratio *= half;
                    auto delta = Duration<PPQ>(std::round(((i + 1)->time_ - i->time_) * ratio));
                    if(delta == 0)
                    {
                        break;
                    }
                    // 求出的脉冲得出的秒数过大
                    if(secondEl - remain > precision)
                    {
                        right -= Duration<PPQ>(std::round(((i + 1)->time_ - i->time_) * ratio));
                    }
                    // 过小
                    else if(remain - secondEl > precision)
                    {
                        right += Duration<PPQ>(std::round(((i + 1)->time_ - i->time_) * ratio));
                    }
                    // 在误差允许的范围内（在进行实际计算时进入这一块的可能性不大）
                    else
                    {
                        return Duration<PPQ>(right.pulse());
                    }
                }
                return Duration<PPQ>(right.pulse());
            }
        }
        auto remain = second - sec;
        auto pulsePerSecond = (Base::cend() - 1)->value_ * PPQ / 60.0;
        auto ret = Duration<PPQ>(((Base::cend() - 1)->time_ - from).duration() + pulsePerSecond * remain);
        return ret;
    }
    double pulseAtTimePoint(double second, double precision = 60.0 / Musec::Audio::Base::maximumTempo / static_cast<double>(PPQ)) const
    {
        return pulseElapsedFrom(TimePoint<PPQ>(0U), second, precision);
    }
};
}
}
}

#endif //MUSEC_DKWTP_TEMPOAUTOMATIONWITHCURVE
