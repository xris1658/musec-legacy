#ifndef MUSEC_NATIVE_WINRTFORWARD
#define MUSEC_NATIVE_WINRTFORWARD

#include <winrt/base.h>

namespace winrt::impl
{
template<typename Async>
auto wait_for(const Async& async, const winrt::Windows::Foundation::TimeSpan& timeout);
}

#endif //MUSEC_NATIVE_WINRTFORWARD
