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

template<typename Function, typename... Args, typename ReturnType = std::result_of_t<Function(Args...)>>
std::tuple<ReturnType, std::int64_t> stopwatch(Function&& function, Args&&... args)
{
    auto start = Musec::Native::currentTimeValueInNanosecond();
    std::tuple<ReturnType, std::int64_t> ret = std::make_tuple<ReturnType, std::int64_t>(std::forward<Function>(function)(std::forward<Args>(args)...), 0);
    // Make sure the time is retrieved AFTER the result is valid - See C++ Order of evaluation for details
    // Is there a better solution?
    auto& [result, time] = ret;
    time = Musec::Native::currentTimeValueInNanosecond() - start;
    return ret;
}

template<typename... Args>
std::int64_t stopwatchVoid(Function<void, Args...>&& function, Args&&... args)
{
    auto start = Musec::Native::currentTimeValueInNanosecond();
    std::forward<Function<void, Args...>>(function)(std::forward<Args>(args)...);
    return Musec::Native::currentTimeValueInNanosecond() - start;
}
}
}

#endif //MUSEC_UTIL_STOPWATCH
