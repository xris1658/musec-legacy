#ifndef MUSEC_AUDIO_BASE_AUTOMATION
#define MUSEC_AUDIO_BASE_AUTOMATION

#include "math/QuadraticFunction.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>
#include <cassert>

namespace Musec
{
namespace Audio
{
namespace Base
{
class AutomationPoint
{
public:
    using TimeType = std::int64_t;
    using ValueType = double;
private:
    using Self = AutomationPoint;
public:
    AutomationPoint(std::int64_t time, double value, double curve) : time_(time), value_(value), curve_(curve)
    {}
    AutomationPoint(const Self&) = default;
    Self& operator=(const Self&) = default;
    ~AutomationPoint() noexcept = default;
public:
    TimeType time() const
    {
        return time_;
    }
    void setTime(TimeType time)
    {
        time_ = time;
    }
    ValueType value() const
    {
        return value_;
    }
    void setValue(ValueType value)
    {
        value_ = value;
    }
    double curve() const
    {
        return curve_;
    }
    void setCurve(double curve)
    {
        curve_ = curve;
    }
public:
    bool operator==(const AutomationPoint& rhs) const
    {
        return time_ == rhs.time_
            && value_ == rhs.value_
            && curve_ == rhs.curve_;
    }
    bool operator!=(const AutomationPoint& rhs) const
    {
        return !(rhs == *this);
    }
private:
    TimeType time_;
    ValueType value_;
    double curve_;
};

bool timeFromPointIsLessThanTime(const AutomationPoint& point, const AutomationPoint::TimeType& time);

struct AutomationMembers
{
    using PointVector = std::vector<AutomationPoint>;
    PointVector points_;
    AutomationPoint::ValueType minValue_;
    AutomationPoint::ValueType maxValue_;
};

class Automation
{
public:
    using TimeType = AutomationPoint::TimeType;
    using ValueType = AutomationPoint::ValueType;
    using Self = Automation;
    using Point = AutomationPoint;
    using PointVector = AutomationMembers::PointVector;
    using PointVectorIterator = typename PointVector::iterator;
    using PointVectorConstIterator = typename PointVector::const_iterator;
public:
    Automation(double minValue = 0.0, double maxValue = 1.0): members_{{}, minValue, maxValue} {}
    Automation(const Self& rhs): members_(rhs.members_) {}
    Automation(Self&& rhs) noexcept: members_(std::move(rhs.members_)) {}
    Self& operator=(const Self& rhs)
    {
        if (this != &rhs)
        {
            members_ = rhs.members_;
        }
        return *this;
    }
    Self& operator=(Self&& rhs) noexcept
    {
        if (this != &rhs)
        {
            members_ = std::move(rhs.members_);
        }
        return *this;
    }
    ~Automation() noexcept {}
    PointVectorConstIterator lowerBound(const AutomationPoint::TimeType& time) const
    {
        return std::lower_bound(points_.cbegin(), points_.cend(), time, timeFromPointIsLessThanTime);
    }
    PointVectorConstIterator upperBound(const AutomationPoint::TimeType& time) const
    {
        auto ret = lowerBound(time);
        if (ret != points_.cend() && ret->time() == time)
        {
            ++ret;
        }
        return ret;
    }
    PointVectorIterator lowerBound(const AutomationPoint::TimeType& time)
    {
        return std::lower_bound(points_.begin(), points_.end(), time, timeFromPointIsLessThanTime);
    }
    PointVectorIterator upperBound(const AutomationPoint::TimeType& time)
    {
        auto ret = lowerBound(time);
        if (ret != points_.cend() && ret->time() == time)
        {
            ++ret;
        }
        return ret;
    }
    PointVectorIterator begin() noexcept
    {
        return points_.begin();
    }
    PointVectorConstIterator cbegin() const noexcept
    {
        return points_.cbegin();
    }
    PointVectorIterator end() noexcept
    {
        return points_.end();
    }
    PointVectorConstIterator cend() const noexcept
    {
        return points_.cend();
    }
    Musec::Math::QuadraticFunction getFunction(PointVectorConstIterator left) const
    {
        if(left == points_.cend())
        {
            throw std::invalid_argument("");
        }
        auto right = left + 1;
        if(right == points_.cend() || left->time() == right->time() || left->value() == right->value())
        {
            return {0, 0, left->value()};
        }
        auto controlX = (left->time() + right->time()) * 0.5;
        auto up = right->curve() * 0.5 + 0.5;
        auto controlY = left->value() < right->value()?
                        left->value() + up * (right->value() - left->value()):
                        right->value() + up * (left->value() - right->value());
        auto dLeft = (controlY - left->value()) / (controlX - left->time());
        auto dRight = (right->value() - controlY) / (right->time() - controlX);
        auto a = (dRight - dLeft) * 0.5 / (right->time() - left->time());
        auto b = dLeft - 2 * a * left->value();
        auto c = left->value() - a * left->time() * left->time() - b * left->time();
        return {a, b, c};
    }
    std::size_t pointCount() const noexcept
    {
        return points_.size();
    }
    bool empty() const noexcept
    {
        return points_.empty();
    }
    AutomationPoint::ValueType minValue() const noexcept
    {
        return minValue_;
    }
    AutomationPoint::ValueType maxValue() const noexcept
    {
        return maxValue_;
    }
    const Point& operator[](const std::size_t index) const
    {
        return points_[index];
    }
    Point& operator[](const std::size_t index)
    {
        return const_cast<Point&>(
            static_cast<const Self&>(*this).operator[](index)
        );
    }
    const Point& at(const std::size_t index) const
    {
        if(index >= pointCount())
        {
            throw std::out_of_range("Error: at() out of range");
        }
        return operator[](index);
    }
    Point& at(const std::size_t index)
    {
        return const_cast<Point&>(
            static_cast<const Self&>(*this).at(index)
        );
    }
    std::size_t pointCountAtTime(const AutomationPoint::TimeType& time) const
    {
        auto lower = lowerBound(time);
        if (lower == points_.end() || lower->time() != time)
        {
            return 0;
        }
        std::size_t ret = 0;
        for (auto iterator = lower; iterator != points_.end() && iterator->time() == time; ++iterator)
        {
            ++ret;
        }
        return ret;
    }
    std::size_t firstPointIndexAtTime(const AutomationPoint::TimeType& time) const
    {
        auto lower = lowerBound(time);
        if(lower == points_.end())
        {
            return points_.size();
        }
        else
        {
            return lower - points_.begin();
        }
    }
    double operator()(const AutomationPoint::TimeType& time, std::size_t index = 0) const
    {
        auto lower = lowerBound(time);
        // the given time point is at back of all points
        if (lower == points_.cend())
        {
            return points_.crbegin()->value();
        }
        else
        {
            // the given time point matches a point
            if (lower->time() == time)
            {
                if (index)
                {
                    auto retIterator = lower + index;
                    if (retIterator->time() != time)
                    {
                        throw std::invalid_argument("");
                    }
                    return retIterator->value();
                }
                else
                {
                    return lower->value();
                }
            }
            else/* if(lower->time() > time)*/
            {
                // lower->time() != time
                // the given time point is ahead of all points, or matches the first point
                if (lower == points_.begin())
                {
                    return lower->value();
                }
                else
                {
                    return getFunction(lower - 1)(time);
                }
            }
        }
    }
    // Inserts a point and returns the index of the point inserted.
    // If there's no point at the given time point, then the second parameter is ignored.
    // If there are points at the given time point, then the point will be inserted there according to `indexInEqualTimePoint`.
    std::size_t insertPoint(Point point, std::size_t indexInEqualTimePoint = 0)
    {
        point.setValue(std::clamp(point.value(), minValue_, maxValue_));
        auto lower = lowerBound(point.time());
        if (lower == points_.end())
        {
            points_.emplace_back(point);
            return points_.size() - 1;
        }
        else if (lower->time() != point.time())
        {
            auto it = points_.emplace(lower, point);
            return it - points_.begin();
        }
        else
        {
            if (indexInEqualTimePoint)
            {
                auto insertBefore = lower + indexInEqualTimePoint;
                auto insertAfter = insertBefore - 1;
                if (insertAfter->time() != point.time())
                {
                    throw std::invalid_argument("");
                }
                auto it = points_.emplace(insertBefore, point);
                return it - points_.begin();
            }
            else
            {
                auto it = points_.emplace(lower, point);
                return it - points_.begin();
            }
        }
    }
    void deletePoint(std::size_t index)
    {
        points_.erase(points_.begin() + index);
    }
    void setValueOfPoint(std::size_t index, double value)
    {
        if(value < minValue_)
        {
            value = minValue_;
        }
        else if(value > maxValue_)
        {
            value = maxValue_;
        }
        (*this)[index].setValue(value);
    }
    std::size_t ifSetTimeOfPoint(std::size_t index, Automation::TimeType time, std::size_t indexInEqualTimePoint = 0)
    {
        auto oldTime = operator[](index).time();
        if(time == oldTime)
        {
            return index;
        }
        if(time < 0)
        {
            time = 0;
        }
        auto lowerBoundIndex = lowerBound(time) + indexInEqualTimePoint - begin();
        if(oldTime < time)
        {
            --lowerBoundIndex;
        }
        assert(lowerBoundIndex >= 0);
        return lowerBoundIndex;
    }
    // Move a point, then returns the new index of the point.
    // There is another parameter `indexInEqualTimePoint`,
    // meaning that this function is similar with `insertPoint`.
    std::size_t movePoint(std::size_t index, AutomationPoint::TimeType time, std::size_t indexInEqualTimePoint = 0)
    {
        auto newIndex = ifSetTimeOfPoint(index, time, indexInEqualTimePoint);
        operator[](index).setTime(time);
        if(newIndex == index)
        {
            return newIndex;
        }
        else if(newIndex > index)
        {
            for(auto i = index; i < newIndex; ++i)
            {
                std::swap(points_[i], points_[i + 1]);
            }
        }
        else
        {
            for(auto i = index; i-- > newIndex; )
            {
                std::swap(points_[i], points_[i + 1]);
            }
        }
        return newIndex;
    }
    void setCurveOfPoint(std::size_t index, double curve)
    {
        if(curve > 1.0)
        {
            curve = 1.0;
        }
        else if(curve < -1.0)
        {
            curve = -1.0;
        }
        (*this)[index].setCurve(curve);
    }
    void clearPoints()
    {
        points_.clear();
    }
private:
    AutomationMembers members_;
    PointVector& points_ = members_.points_;
    decltype(AutomationMembers::minValue_)& minValue_ = members_.minValue_;
    decltype(AutomationMembers::maxValue_)& maxValue_ = members_.maxValue_;
};
}
}
}

#endif //MUSEC_AUDIO_BASE_AUTOMATION
