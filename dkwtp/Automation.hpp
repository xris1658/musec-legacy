#ifndef MUSEC_DKWTP_AUTOMATION
#define MUSEC_DKWTP_AUTOMATION

#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Musec
{
namespace DKWTP
{
template<typename T, typename V>
struct AutomationPoint
{
    T time_;
    V value_;
};

template<typename T, typename V>
bool timeFromPointIsLessThanTime(const AutomationPoint<T, V>& point, const T time)
{
    return point.time_ < time;
}

template<typename T, typename V>
class Automation
{
public:
    using TimeType = T;
    using ValueType = V;
    using Self = Automation<T, V>;
    using Point = AutomationPoint<T, V>;
public:
    Automation(): points_() {}
    Automation(const Self& rhs): points_(rhs.points_) {}
    Automation(Self&& rhs) noexcept: points_(std::move(rhs.points_)) {}
    Automation& operator=(const Self& rhs)
    {
        if (this != &rhs)
        {
            points_ = rhs.points_;
        }
    }
    Automation& operator=(Self&& rhs) noexcept
    {
        if (this != &rhs)
        {
            points_ = std::move(rhs.points_);
        }
    }
    ~Automation() noexcept {}
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
                static_cast<const Point&>(*this)[index]
        );
    }
    std::size_t pointCount(const T time) const
    {
        auto lower = std::lower_bound(points_.begin(), points_.end(), time, timeFromPointIsLessThanTime<T, V>);
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
    V operator()(const T time, std::size_t index = 0) const
    {
        auto lower = std::lower_bound(points_.begin(), points_.end(), time, timeFromPointIsLessThanTime<T, V>);
        if (lower == points_.end())
        {
            return points_.rbegin()->value_;
        }
        else
        {
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
                if (lower == points_.begin())
                {
                    return lower->value_;
                }
                else
                {
                    // TODO: 计算值。由于曲线类型还没有决定，此处只按线性插值计算
                    auto left = lower - 1;
                    auto& right = lower;
                    return (right->value_ - left->value_) / (right->time_ - left->time_) * (time - left->time_) + left->value_;
                }
            }
        }
    }
    void insertPoint(const Point& point, std::size_t indexInEqualTimePoint = 0)
    {
        auto lower = std::lower_bound(points_.begin(), points_.end(), point.time_, timeFromPointIsLessThanTime<T, V>);
        if (lower == points_.end())
        {
            points_.emplace_back(point);
        }
        else if (lower->time_ != point.time_)
        {
            points_.emplace(lower + 1, point);
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
                points_.emplace(insertBefore, point);
            }
            else
            {
                points_.emplace(lower, point);
            }
        }
    }
    void deletePoint(std::size_t index)
    {
        points_.erase(points_.begin() + index);
    }
    void clearPoints()
    {
        points_.clear();
    }
private:
    std::vector<Point> points_;
};
}
}

#endif //MUSEC_DKWTP_AUTOMATION
