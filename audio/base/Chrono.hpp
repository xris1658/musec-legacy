#ifndef MUSEC_DKWTP_CHRONO
#define MUSEC_DKWTP_CHRONO

#include <cstddef>
#include <cstdint>

namespace Musec
{
namespace Audio
{
namespace Base
{
template<std::size_t PPQ>
class Duration;

template<std::size_t PPQ>
class TimePoint
{
    static_assert(PPQ % 24 == 0);
public:
    explicit TimePoint(std::int64_t pulse = 0U): pulse_(pulse) {}
    TimePoint(const TimePoint<PPQ>&) = default;
    TimePoint& operator=(const TimePoint<PPQ>&) = default;
    TimePoint(TimePoint<PPQ>&&) noexcept = default;
    TimePoint& operator=(TimePoint<PPQ>&&) noexcept = default;
    ~TimePoint() noexcept {}
public:
    std::int64_t pulse() const
    {
        return pulse_;
    }
    operator std::int64_t() const
    {
        return pulse_;
    }
    constexpr std::size_t ppq()
    {
        return PPQ;
    }
public:
    TimePoint<PPQ>& operator++()
    {
        ++pulse_;
        return *this;
    }
    TimePoint<PPQ>& operator--()
    {
        --pulse_;
        return *this;
    }
    const TimePoint<PPQ> operator--(int)
    {
        TimePoint<PPQ> ret = *this;
        operator--();
        return ret;
    }
    const TimePoint<PPQ> operator++(int)
    {
        TimePoint<PPQ> ret = *this;
        operator++();
        return ret;
    }
    TimePoint<PPQ>& operator+=(const Duration<PPQ>& duration);
    TimePoint<PPQ>& operator-=(const Duration<PPQ>& duration);
public:
    template<std::size_t PPQ2>
    explicit operator TimePoint<PPQ2>()
    {
        return TimePoint<PPQ2>(pulse_ * PPQ2 / PPQ);
    }
private:
    std::int64_t pulse_;
};

template<std::size_t PPQ>
class Duration
{
    static_assert(PPQ % 24 == 0);
public:
    explicit Duration(std::int64_t duration): duration_(duration) {}
    Duration(const Duration<PPQ>&) = default;
    Duration& operator=(const Duration<PPQ>&)= default;
    Duration(Duration<PPQ>&&) noexcept = default;
    Duration& operator=(Duration<PPQ>&&) noexcept = default;
    ~Duration() noexcept {}
public:
    std::int64_t duration() const
    {
        return duration_;
    }
    operator std::int64_t() const
    {
        return duration_;
    }
    constexpr std::size_t ppq()
    {
        return PPQ;
    }
public:
    Duration<PPQ>& operator+=(const Duration<PPQ>& rhs)
    {
        duration_ += rhs.duration_;
        return *this;
    }
    Duration<PPQ>& operator-=(const Duration<PPQ>& rhs)
    {
        duration_ -= rhs.duration_;
        return *this;
    }
public:
    template<std::size_t PPQ2>
    explicit operator Duration<PPQ2>()
    {
        return Duration<PPQ2>(duration_ * PPQ2 / PPQ);
    }
private:
    std::int64_t duration_;
};

// TimePoint 比较运算符
template<std::size_t PPQ>
bool operator==(const TimePoint<PPQ>& lhs, const TimePoint<PPQ>& rhs)
{
    return lhs.pulse() == rhs.pulse();
}

template<std::size_t PPQ>
bool operator!=(const TimePoint<PPQ>& lhs, const TimePoint<PPQ>& rhs)
{
    return !(lhs == rhs);
}

template<std::size_t PPQ>
bool operator<(const TimePoint<PPQ>& lhs, const TimePoint<PPQ>& rhs)
{
    return lhs.pulse() < rhs.pulse();
}

template<std::size_t PPQ>
bool operator>(const TimePoint<PPQ>& lhs, const TimePoint<PPQ>& rhs)
{
    return rhs < lhs;
}

template<std::size_t PPQ>
bool operator<=(const TimePoint<PPQ>& lhs, const TimePoint<PPQ>& rhs)
{
    return !(lhs > rhs);
}

template<std::size_t PPQ>
bool operator>=(const TimePoint<PPQ>& lhs, const TimePoint<PPQ>& rhs)
{
    return !(lhs < rhs);
}

// Duration 比较运算符
template<std::size_t PPQ>
bool operator==(const Duration<PPQ>& lhs, const Duration<PPQ>& rhs)
{
    return lhs.duration() == rhs.duration();
}

template<std::size_t PPQ>
bool operator!=(const Duration<PPQ>& lhs, const Duration<PPQ>& rhs)
{
    return !(lhs == rhs);
}

template<std::size_t PPQ>
bool operator<(const Duration<PPQ>& lhs, const Duration<PPQ>& rhs)
{
    return lhs.duration() < rhs.duration();
}

template<std::size_t PPQ>
bool operator>(const Duration<PPQ>& lhs, const Duration<PPQ>& rhs)
{
    return rhs < lhs;
}

template<std::size_t PPQ>
bool operator<=(const Duration<PPQ>& lhs, const Duration<PPQ>& rhs)
{
    return !(lhs > rhs);
}

template<std::size_t PPQ>
bool operator>=(const Duration<PPQ>& lhs, const Duration<PPQ>& rhs)
{
    return !(lhs < rhs);
}

// TimePoint + Duration
template<std::size_t PPQ>
TimePoint<PPQ> operator+(const TimePoint<PPQ>& lhs, const Duration<PPQ>& rhs)
{
    return TimePoint<PPQ>(lhs.pulse() + rhs.duration());
}

// TimePoint - Duration
template<std::size_t PPQ>
TimePoint<PPQ> operator-(const TimePoint<PPQ>& lhs, const Duration<PPQ>& rhs)
{
    return TimePoint<PPQ>(lhs.pulse() - rhs.duration());
}

// TimePoint - TimePoint
template<std::size_t PPQ>
Duration<PPQ> operator-(const TimePoint<PPQ>& lhs, const TimePoint<PPQ>& rhs)
{
    return Duration<PPQ>(lhs.pulse() - rhs.pulse());
}

// Duration + Duration
template<std::size_t PPQ>
Duration<PPQ> operator+(const Duration<PPQ>& lhs, const Duration<PPQ>& rhs)
{
    return Duration<PPQ>(lhs.duration() + rhs.duration());
}

// Duration - Duration
template<std::size_t PPQ>
Duration<PPQ> operator-(const Duration<PPQ>& lhs, const Duration<PPQ>& rhs)
{
    return Duration<PPQ>(lhs.duration() - rhs.duration());
}

template<std::size_t PPQ>
TimePoint<PPQ>& TimePoint<PPQ>::operator+=(const Duration<PPQ>& duration)
{
    pulse_ += duration.duration();
    return *this;
}

template<std::size_t PPQ>
TimePoint<PPQ>& TimePoint<PPQ>::operator-=(const Duration<PPQ>& duration)
{
    pulse_ -= duration.duration();
    return *this;
}
}
}
}

#endif //MUSEC_DKWTP_CHRONO
