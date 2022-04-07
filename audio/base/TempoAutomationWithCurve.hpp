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
                auto function = Base::getFunction(beforeStart);
                function.a *= PPQ;
                function.b *= PPQ;
                function.c *= PPQ;
                ret = Musec::Math::quadraticFunctionInvertIntegration(
                    function, from, to
                ) * 60.0;
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
            auto function = Base::getFunction(notBeforeStart - 1);
            function.a *= PPQ;
            function.b *= PPQ;
            function.c *= PPQ;
            ret += Musec::Math::quadraticFunctionInvertIntegration(
                function, from, notBeforeStart->time_
            ) * 60.0;
        }
        auto beforeEnd = notBeforeEnd - 1;
        // Step 2 of 3: --point--point--
        for(auto it = notBeforeStart; it < beforeEnd; ++it)
        {
            auto function = Base::getFunction(it);
            function.a *= PPQ;
            function.b *= PPQ;
            function.c *= PPQ;
            ret += Musec::Math::quadraticFunctionInvertIntegration(
                function, it->time_, (it + 1)->time_
            ) * 60.0;
        }
        // Step 3 of 3: --to
        if(notBeforeEnd == Base::cend())
        {
            ret += secondElapsed(beforeEnd->value_, to - beforeEnd->time_);
        }
        else
        {
            auto function = Base::getFunction(beforeEnd);
            function.a *= PPQ;
            function.b *= PPQ;
            function.c *= PPQ;
            ret += Musec::Math::quadraticFunctionInvertIntegration(
                function, beforeEnd->time_, to
            ) * 60.0;
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
    double pulseElapsedFrom(const TimePoint<PPQ>& from, double second)
    {
        if(second == 0)
        {
            return 0;
        }
        // from 右侧的点，或者与 from 重合的点
        auto lowerBound = Base::lowerBound(from);

        // TODO: 在此处完成实际时间到节拍的转换
        // 方法未知：硬算？二分法？或者是别的？
        // 我大概得去看看数值分析的内容。
    }
    double pulseAtTimePoint(double second)
    {
        return pulseElapsedFrom(TimePoint<PPQ>(0U), second);
    }
};


}
}
}

#endif //MUSEC_DKWTP_TEMPOAUTOMATIONWITHCURVE
