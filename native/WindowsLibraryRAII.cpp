#include "WindowsLibraryRAII.hpp"

#include <QDebug>

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
    std::wstring widePath = path.toStdWString();
    module_ = LoadLibraryExW(widePath.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    if(module_ == NULL)
    {
        auto error = GetLastError();
        throw error;
    }
    else
    {
    }
}

WindowsLibraryRAII::WindowsLibraryRAII(WindowsLibraryRAII&& rhs): module_(NULL)
{
    std::swap(module_, rhs.module_);
}

WindowsLibraryRAII& WindowsLibraryRAII::operator=(WindowsLibraryRAII&& rhs)
{
    if(module_ != rhs.module_)
    {
        std::swap(module_, rhs.module_);
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

WindowsLibraryRAII::operator bool() const
{
    return module_ != NULL;
}
}
