#include "WindowsLibraryRAII.hpp"

#include <Windows.h>
#include <libloaderapi.h>

#include <string>
#include <utility>

namespace Musec::Native
{
WindowsLibraryRAII::WindowsLibraryRAII(): module_(NULL)
{
    //
}

WindowsLibraryRAII::WindowsLibraryRAII(const QString& path)
{
#ifndef NDEBUG
    path_ = path;
#endif
    std::wstring widePath = path.toStdWString();
    module_ = LoadLibraryExW(widePath.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    if(module_ == NULL)
    {
        throw GetLastError();
    }
}

WindowsLibraryRAII::WindowsLibraryRAII(WindowsLibraryRAII&& rhs): module_(NULL)
{
    std::swap(module_, rhs.module_);
#ifndef NDEBUG
    path_ = std::move(rhs.path_);
#endif
}

WindowsLibraryRAII& WindowsLibraryRAII::operator=(WindowsLibraryRAII&& rhs)
{
    if(module_ != rhs.module_)
    {
        std::swap(module_, rhs.module_);
#ifndef NDEBUG
        std::swap(path_, rhs.path_);
#endif
    }
    return *this;
}

WindowsLibraryRAII::~WindowsLibraryRAII() noexcept
{
    if(module_)
    {
        FreeLibrary(module_);
    }
}

HMODULE WindowsLibraryRAII::module() const
{
    return module_;
}
}
