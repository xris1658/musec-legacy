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
template<std::size_t PPQ>
using TempoAutomationPointWithCurve = AutomationPointWithCurve<TimePoint<PPQ>, double>;

template<std::size_t PPQ>
class TempoAutomationWithCurve: public AutomationWithCurve<TimePoint<PPQ>, double>
{
using Base = AutomationWithCurve<TimePoint<PPQ>, double>;
private:
bool validatePoint(typename Base::PointVectorConstIterator left)
{
    auto right = left + 1;
    auto controlX = (left->time_ + right->time_) * 0.5;
    auto up = right->curve_ * 0.5 + 0.5;
    auto controlY = left->value_ < right->value_?
                    left->value_ + up * (right->value_ - left->value_):
                    right->value_ + up * (left->value_ - right->value_);
    auto dLeft = (controlY - left->value_) / (controlX - left->time_);
    auto dRight = (right->value_ - controlY) / (right->time_ - controlX);
    auto a = (dRight - dLeft) * 0.5 / (right->time_ - left->time_);
    auto b = dLeft - 2 * a * left->value_;
    auto c = left->value_ - a * left->time_ * left->time_ - b * left->time_;
    return b * b - 4 * a * c != 0;
}
double secondElapsed(double bpm, const Duration<PPQ>& duration) const
{
    return duration / (bpm * PPQ) * 60.0;
}
public:
    std::size_t insertPoint(const typename Base::Point& point, std::size_t indexInEqualTimePoint = 0) override
    {
        auto ret = Base::insertPoint(point, indexInEqualTimePoint);
    // 插入点之后判断二次函数的 delta 是否为零

    // 首先找点
        auto pointIterator = Base::lowerBound(point.time_);
        if (Base::pointCountAtTime(point.time_) > 1)
        {
            pointIterator += indexInEqualTimePoint;
        }
        // 如果插入的点不是自动化包络中的第一个点...
        if (pointIterator != Base::begin())
        {
            auto& right = pointIterator;
            auto left = pointIterator - 1;
            // 且插入的点前的点的时间与这个插入的点的时间不相等
            if (left->time_ != right->time_ && right->curve_ < 0 && right->curve_ != -1)
            {
                constexpr auto tweak = 1 / 64.0;
                while (!validatePoint(left))
                {
                    right->curve_ += tweak;
                }
            }
        }
        // 如果插入的点不是自动化包络中的最后一个点...
        if (ret != Base::pointCount() - 1)
        {
            auto& left = pointIterator;
            auto right = pointIterator + 1;
            // 且插入的点后的点的时间与这个插入的点的时间不相等
            if (left->time_ != right->time_ && right->curve_ < 0 && right->curve_ != -1)
            {
                constexpr auto tweak = 1 / 64.0;
                while (!validatePoint(left))
                {
                    right->curve_ += tweak;
                }
            }
        }
        return ret;
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
};

}
}
}

#endif //MUSEC_DKWTP_TEMPOAUTOMATIONWITHCURVE
