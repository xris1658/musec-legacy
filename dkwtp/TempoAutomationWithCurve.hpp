#ifndef MUSEC_DKWTP_TEMPOAUTOMATIONWITHCURVE
#define MUSEC_DKWTP_TEMPOAUTOMATIONWITHCURVE

#include "dkwtp/AutomationWithCurve.hpp"
#include "dkwtp/Chrono.hpp"
#include "math/Integration.hpp"
#include "math/QuadraticFunction.hpp"

#include <cmath>
#include <stdexcept>

namespace Musec
{
namespace DKWTP
{
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
            );
        }
        auto beforeEnd = notBeforeEnd - 1;
        // Step 2 of 3: --point--point--
        for(auto it = notBeforeStart; it != beforeEnd; ++it)
        {
            auto function = Base::getFunction(it);
            function.a *= PPQ;
            function.b *= PPQ;
            function.c *= PPQ;
            ret += Musec::Math::quadraticFunctionInvertIntegration(
                function, it->time_, (it + 1)->time_
            );
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
            );
        }
        return ret * 60.0;
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

#endif //MUSEC_DKWTP_TEMPOAUTOMATIONWITHCURVE
