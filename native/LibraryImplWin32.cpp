#include "LibraryImpl.hpp"

#include <libloaderapi.h>

namespace Musec::Native
{
LibraryImpl::LibraryImpl(): handle_(NULL)
{
}

LibraryImpl::LibraryImpl(const QString& path)
{
    auto widePath = path.toStdWString();
    handle_ = reinterpret_cast<decltype(handle_)>(
        LoadLibraryExW(widePath.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH));
    if(handle_ == NULL)
    {
        errorCode_ = GetLastError();
        handle_ = NULL;
    }
}

LibraryImpl::LibraryImpl(LibraryImpl&& rhs) noexcept: handle_(NULL)
{
    std::swap(handle_, rhs.handle_);
}

LibraryImpl& LibraryImpl::operator=(LibraryImpl&& rhs) noexcept
{
    if(handle_ != rhs.handle_)
    {
        std::swap(handle_, rhs.handle_);
    }
    return *this;
}

LibraryImpl::~LibraryImpl() noexcept
{
    if(handle_)
    {
        FreeLibrary(reinterpret_cast<HMODULE>(handle_));
    }
}

ErrorCodeType LibraryImpl::errorCode() const
{
    return errorCode_;
}

Musec::Native::Library::ExportType LibraryImpl::getExport(const char* name)
{
    return GetProcAddress(reinterpret_cast<HMODULE>(handle_), name);
}
}