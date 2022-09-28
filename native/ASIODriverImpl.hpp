#ifndef MUSEC_NATIVE_ASIODRIVERIMPL
#define MUSEC_NATIVE_ASIODRIVERIMPL

#include "audio/driver/ASIODriverBasicInfo.hpp"
#include "audio/driver/ASIODriverForward.hpp"
#include "audio/driver/ASIODriverStreamInfo.hpp"

namespace Musec
{
namespace Native
{
using namespace Musec::Audio::Driver;

QList<Musec::Audio::Driver::ASIODriverBasicInfo> enumerateASIODriverInfo();

// ASIO driver pImpl class
class ASIODriverImpl
{
public:
    ASIODriverImpl();
    explicit ASIODriverImpl(const ASIODriverBasicInfo& info);
    ASIODriverImpl(const ASIODriverImpl&) = delete;
    ASIODriverImpl& operator=(const ASIODriverImpl&) = delete;
    ASIODriverImpl(ASIODriverImpl&&) noexcept;
    ASIODriverImpl& operator=(ASIODriverImpl&&) noexcept;
    ~ASIODriverImpl();
public:
    ASIODriverBasicInfo driverInfo() const;
    IASIO* driver() const
    {
        return driver_;
    }
    operator IASIO*() const
    {
        return driver_;
    }
    IASIO* operator*() const
    {
        return driver_;
    }
    IASIO* operator->() const
    {
        return driver_;
    }
public:
    void swap(ASIODriverImpl& rhs);
private:
    ASIODriverBasicInfo driverInfo_;
    IASIO* driver_; // Is shared_ptr needed?
};
}
}

namespace std
{
template<>
void swap(Musec::Native::ASIODriverImpl& lhs, Musec::Native::ASIODriverImpl& rhs)
    noexcept(std::is_move_assignable_v<Musec::Native::ASIODriverImpl>);
}

#endif //MUSEC_NATIVE_ASIODRIVERIMPL
