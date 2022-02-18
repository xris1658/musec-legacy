#ifndef MUSEC_UTIL_FUNCTORINITIALIZER
#define MUSEC_UTIL_FUNCTORINITIALIZER

#include <utility>

namespace Musec
{
namespace Util
{
template<typename FunctorType> class FunctorInitializer
{
    // 仿函数最好是 Lambda 表达式，且不是复制品
    static_assert(std::is_same_v<std::result_of_t<FunctorType>, void>,
        "ERROR: FunctorType must return void");
private:
    using Self = FunctorInitializer<FunctorType>;
private:
    FunctorInitializer(FunctorType&& functor)
    {
        std::forward<FunctorType>(functor)();
    }
public:
    static void init(FunctorType&& functor)
    {
        static Self singleton(std::forward<FunctorType>(functor));
    }
};
}
}

#endif // MUSEC_UTIL_FUNCTORINITIALIZER
