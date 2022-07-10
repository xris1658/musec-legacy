#include "CLAPHost.hpp"

#include "base/Constants.hpp"

namespace Musec::Audio::Host
{
namespace Impl
{
template<typename T>
class CLAPStruct
{
private:
    CLAPStruct() { static_assert(false); }
public:
    static CLAPStruct<T>& instance()
    {
        static CLAPStruct<T> ret;
        return ret;
    }
public:
    operator T&() { return struct_; }
private:
    T struct_;
};

const void* CLAPHostGetExtension(const clap_host* host, const char* extension)
{
    return nullptr;
}

void CLAPHostRequestCallback(const clap_host* host) {}

void CLAPHostRequestProcess(const clap_host* host) {}

void CLAPHostRequestRestart(const clap_host* host) {}

template<>
CLAPStruct<clap_host>::CLAPStruct()
{
    struct_.name = Musec::Base::ProductName;
    struct_.vendor = Musec::Base::CompanyName;
    struct_.version = Musec::Base::Version;
    struct_.url = Musec::Base::Url;
    struct_.get_extension = &CLAPHostGetExtension;
    struct_.request_callback = &CLAPHostRequestCallback;
    struct_.request_process = &CLAPHostRequestProcess;
    struct_.request_restart = &CLAPHostRequestRestart;
}

}
const clap_host& AppCLAPHost()
{
    return Impl::CLAPStruct<clap_host>::instance();
}

}
