#ifndef MUSEC_UTIL_STOPWATCH
#define MUSEC_UTIL_STOPWATCH

#include "native/Native.hpp"

#include <tuple>

namespace Musec
{
namespace Util
{
template<typename ReturnType, typename... Args>
using Function = ReturnType(Args...);

template<typename Function, typename... Args, typename ReturnType = std::result_of_t<Function>>
std::tuple<ReturnType, std::int64_t> stopwatch(Function&& function, Args&&... args)
{
    auto start = Musec::Native::currentTimeInNanosecond();
    return std::make_tuple<ReturnType, std::int64_t>(std::forward<Function>(function)(std::forward<Args>(args)...),
        Musec::Native::currentTimeInNanosecond() - start);
}

template<typename... Args>
std::int64_t stopwatchVoid(Function < void, Args...>&& function, Args&&... args)
{
    auto start = Musec::Native::currentTimeInNanosecond();
    std::forward<Function<void, Args...>>(function)(std::forward<Args>(args)...);
    return Musec::Native::currentTimeInNanosecond() - start;
}
}
}

#endif //MUSEC_UTIL_STOPWATCH
