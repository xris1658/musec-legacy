#ifndef MUSEC_DKWTP_AUTOMATIONWITHCURVE
#define MUSEC_DKWTP_AUTOMATIONWITHCURVE

#include "math/QuadraticFunction.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Musec
{
namespace Audio
{
namespace Base
{
template<typename T, typename V>
struct AutomationPointWithCurve
{
    T time_;
    V value_;
    double curve_;
};

constexpr int AutomationPointWithCurveColumnCount = 3;

template<typename T, typename V>
bool timeFromPointWithCurveIsLessThanTime(const AutomationPointWithCurve<T, V>& point, const T& time)
{
    return point.time_ < time;
}

template<typename T, typename V>
class AutomationWithCurve
{
public:
    using TimeType = T;
    using ValueType = V;
    using Self = AutomationWithCurve<T, V>;
    using Point = AutomationPointWithCurve<T, V>;
protected:
    using PointVector = std::vector<Point>;
    using PointVectorIterator = typename PointVector::iterator;
    using PointVectorConstIterator = typename PointVector::const_iterator;
public:
    AutomationWithCurve(double minValue = 0.0, double maxValue = 1.0): points_(), minValue_(minValue), maxValue_(maxValue) {}
    AutomationWithCurve(const Self& rhs): points_(rhs.points_), minValue_(rhs.minValue_), maxValue_(rhs.maxValue) {}
    AutomationWithCurve(Self&& rhs) noexcept: points_(std::move(rhs.points_)), minValue_(rhs.minValue_), maxValue_(rhs.maxValue_) {}
    Self& operator=(const Self& rhs)
    {
        if (this != &rhs)
        {
            points_ = rhs.points_;
            minValue_ = rhs.minValue_;
            maxValue_ = rhs.maxValue_;
        }
        return *this;
    }
    Self& operator=(Self&& rhs) noexcept
    {
        if (this != &rhs)
        {
            points_ = std::move(rhs.points_);
            minValue_ = rhs.minValue_;
            maxValue_ = rhs.maxValue_;
        }
        return *this;
    }
    ~AutomationWithCurve() noexcept {}
protected:
    PointVectorConstIterator lowerBound(const T& time) const
    {
        return std::lower_bound(points_.cbegin(), points_.cend(), time, timeFromPointWithCurveIsLessThanTime<T, V>);
    }
    PointVectorConstIterator upperBound(const T& time) const
    {
        auto ret = lowerBound(time);
        if (ret != points_.cend() && ret->time_ == time)
        {
            ++ret;
        }
        return ret;
    }
    PointVectorIterator lowerBound(const T& time)
    {
        return std::lower_bound(points_.begin(), points_.end(), time, timeFromPointWithCurveIsLessThanTime<T, V>);
    }
    PointVectorIterator upperBound(const T& time)
    {
        auto ret = lowerBound(time);
        if (ret != points_.cend() && ret->time_ == time)
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
        if(right == points_.cend() || left->time_ == right->time_ || left->value_ == right->value_)
        {
            return {0, 0, left->value_};
        }
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
        return {a, b, c};
    }
public:
    std::size_t pointCount() const noexcept
    {
        return points_.size();
    }
    bool empty() const noexcept
    {
        return points_.empty();
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
    std::size_t pointCountAtTime(const T& time) const
    {
        auto lower = lowerBound(time);
        if (lower == points_.end() || lower->time_ != time)
        {
            return 0;
        }
        std::size_t ret = 0;
        for (auto iterator = lower; iterator != points_.end() && iterator->time_ == time; ++iterator)
        {
            ++ret;
        }
        return ret;
    }
    std::size_t firstPointIndexAtTime(const T& time) const
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
    V operator()(const T& time, std::size_t index = 0) const
    {
        auto lower = lowerBound(time);
        // 给定的时刻在所有点之后
        if (lower == points_.cend())
        {
            return points_.crbegin()->value_;
        }
        else
        {
            // 给定的时刻与点重合
            if (lower->time_ == time)
            {
                if (index)
                {
                    auto retIterator = lower + index;
                    if (retIterator->time_ != time)
                    {
                        throw std::invalid_argument("");
                    }
                    return retIterator->value_;
                }
                else
                {
                    return lower->value_;
                }
            }
            else/* if(lower->time_ > time)*/
            {
                // lower->time_ != time
                // 给定的时刻在所有点之前，或与第一个点重合
                if (lower == points_.begin())
                {
                    return lower->value_;
                }
                else
                {
                    return getFunction(lower - 1)(time);
                }
            }
        }
    }
    // 插入点，并返回点插入的索引。
    // 如果所在时间点没有其他点存在，则直接插入，忽略第二个参数。
    // 如果所在时间点有其他点存在，则按照给定的索引值将点插入到合适的位置。
    std::size_t insertPoint(Point point, std::size_t indexInEqualTimePoint = 0)
    {
        if(point.value_ < minValue_)
        {
            point.value_ = minValue_;
        }
        else if(point.value_ > maxValue_)
        {
            point.value_ = maxValue_;
        }
        auto lower = lowerBound(point.time_);
        if (lower == points_.end())
        {
            points_.emplace_back(point);
            return points_.size() - 1;
        }
        else if (lower->time_ != point.time_)
        {
            auto it = points_.emplace(lower + 1, point);
            return it - points_.begin();
        }
        else
        {
            if (indexInEqualTimePoint)
            {
                auto insertBefore = lower + indexInEqualTimePoint;
                auto insertAfter = insertBefore - 1;
                if (insertAfter->time_ != point.time_)
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
        auto right = begin() + index;
        right->curve_ = 0;
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
    }
    void clearPoints()
    {
        points_.clear();
    }
private:
    PointVector points_;
    double minValue_;
    double maxValue_;
};
}
}
}

#endif //MUSEC_DKWTP_AUTOMATIONWITHCURVE
