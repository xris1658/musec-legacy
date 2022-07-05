#include "Automation.hpp"

namespace Musec::Audio::Base
{
bool timeFromPointIsLessThanTime(const AutomationPoint& point, const AutomationPoint::TimeType& time)
{
    return point.time() < time;
}
}