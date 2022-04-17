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
public:
    TempoAutomationWithCurve(): Base(minimumTempo, maximumTempo) {}
private:
    // 给定速度和经过的脉冲数，求经过的实际时间，单位为秒。
    double secondElapsed(double bpm, const Duration<PPQ>& duration) const
    {
        return duration / (bpm * PPQ) * 60.0;
    }
private:
    // 给定一段速度自动化曲线中的一段和启动终止脉冲数，求经过的实际时间。假设启动和终止脉冲在曲线一段的范围内。
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
    double secondElapsed(typename Base::PointVectorConstIterator left, double from, double to) const
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
    // 求曲线的 PPQ 值。
    std::size_t ppq() const
    {
        return PPQ;
    }
    // 给定启动和终止脉冲数，求速度自动化经过的实际时间。
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
                ret = secondElapsed(beforeStart, from, to);
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
    // 求速度自动化从零到终止脉冲数经过的实际时间。
    double secondElapsedFromZero(const TimePoint<PPQ>& to) const
    {
        return secondElapsed(TimePoint<PPQ>(0), to);
    }
    // 求速度自动化在某一个脉冲到下一个脉冲经过的实际时间。
    double secondElapsedInPulse(const TimePoint<PPQ>& pulse) const
    {
        return secondElapsed(pulse, TimePoint<PPQ>(pulse.pulse() + 1));
    }
    // 给定某个脉冲开始经过一定实际时间后到达的脉冲数。
    Duration<PPQ> pulseElapsedFrom(const TimePoint<PPQ>& from, double second, double precision = 60.0 / Musec::Audio::Base::maximumTempo / static_cast<double>(PPQ)) const
    {
        if(second == 0)
        {
            return Duration<PPQ>(from.pulse());
        }
        // from 右侧的点，或者与 from 重合的点
        auto lowerBound = Base::lowerBound(from);
        double sec = from == lowerBound->time_? 0: secondElapsed(from, lowerBound->time_);
        for(auto i = lowerBound; i != Base::cend() - 1; ++i)
        {
            auto durationFromThisToNext = secondElapsed(i, i->time_, (i + 1)->time_);
            // --o--o--x--
            if(sec + durationFromThisToNext < second)
            {
                sec += durationFromThisToNext;
            }
            // --o--o(x)--
            else if(sec + durationFromThisToNext == second)
            {
                return Duration<PPQ>((i + 1)->time_);
            }
            // --o--x--o--
            else
            {
                // 点两边的速度值相等，用不着上迭代法
                if(i->value_ == (i + 1)->value_)
                {
                    auto pulsePerSecond = i->value_ * PPQ / 60.0;
                    return Duration<PPQ>(i->time_.pulse() + pulsePerSecond * (second - sec));
                }
                else
                {
                    auto pointIteratorAtRight = i + 1;
                    auto remain = second - sec;
                    double leftRatio = 0.0;
                    double rightRatio = 1.0;
                    constexpr double half = 0.5;
                    // 除以 2.0 比乘 0.5 慢
                    double delta = half;
                    // (leftRatio + rightRatio) / 2
                    double ratio = (leftRatio + rightRatio) * half;
                    // ratio 对应的时间点（浮点）
                    auto timePoint = i->time_ + (pointIteratorAtRight->time_ - i->time_) * ratio;
                    // leftRatio 对应的时间点（浮点）
                    auto leftRange = i->time_ + (pointIteratorAtRight->time_ - i->time_) * leftRatio;
                    // rightRatio 对应的时间点（浮点）
                    auto rightRange = i->time_ + (pointIteratorAtRight->time_ - i->time_) * rightRatio;
                    while(rightRange - leftRange >= 1)
                    {
                        // 偏右
                        if(remain - secondElapsed(i, static_cast<double>(i->time_), timePoint) > precision * half)
                        {
                            leftRatio += delta;
                        }
                        // 偏左
                        else if(secondElapsed(i, static_cast<double>(i->time_), timePoint) - remain > precision * half)
                        {
                            rightRatio -= delta;
                        }
                        // 在误差范围内
                        else
                        {
                            break;
                        }
                        delta *= half;
                        ratio = (leftRatio + rightRatio) * half;
                        timePoint = i->time_ + (pointIteratorAtRight->time_ - i->time_) * ratio;
                        leftRange = i->time_ + (pointIteratorAtRight->time_ - i->time_) * leftRatio;
                        rightRange = i->time_ + (pointIteratorAtRight->time_ - i->time_) * rightRatio;
                    }
                    auto floor = Duration<PPQ>(std::floor(timePoint));
                    auto ceiling = Duration<PPQ>(std::ceil(timePoint));
                    auto ret = floor;
                    auto secondFloor = secondElapsed(from, floor);
                    auto secondCeiling = secondElapsed(from, ceiling);
                    if (std::floor(timePoint) < leftRange || second - secondFloor >= secondCeiling - second)
                    {
                        ret = ceiling;
                    }
                    auto secondElapsedToRet = secondElapsed(from, TimePoint<PPQ>(ret.duration()));
                    auto retUp = Duration<PPQ>(ret.duration() + 1);
                    auto secondElapsedToRetUp = secondElapsed(from, TimePoint<PPQ>(retUp.duration()));
                    if(std::abs(secondElapsedToRetUp - second) < std::abs(secondElapsedToRet - second))
                    {
                        ret = retUp;
                    }
                    if(ret.duration() != 0)
                    {
                        auto retDown = Duration<PPQ>(ret.duration() - 1);
                        auto secondElapsedToRetDown = secondElapsed(from, TimePoint<PPQ>(retDown.duration()));
                        if(std::abs(secondElapsedToRetDown - second) < std::abs(secondElapsedToRet - second))
                        {
                            ret = retDown;
                        }
                    }
                    return ret;
                }
            }
        }
        // 脉冲值在最后一个点之后
        auto remain = second - sec;
        auto pulsePerSecond = (Base::cend() - 1)->value_ * PPQ / 60.0;
        auto ret = Duration<PPQ>(((Base::cend() - 1)->time_ - from).duration() + pulsePerSecond * remain);
        return ret;
    }
    // 求速度自动化在特定时刻下经过的脉冲数。
    Duration<PPQ> pulseAtTimePoint(double second, double precision = 60.0 / Musec::Audio::Base::maximumTempo / static_cast<double>(PPQ)) const
    {
        return pulseElapsedFrom(TimePoint<PPQ>(0U), second, precision);
    }
};
}
}
}

#endif //MUSEC_DKWTP_TEMPOAUTOMATIONWITHCURVE
