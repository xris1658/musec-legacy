#include "WindowsLibrary.hpp"

#include <Windows.h>
#include <libloaderapi.h>

#include <string>
#include <utility>

namespace Musec::Native
{
namespace Impl
{
ExportType getExport(const WindowsLibrary& library, const char* name)
{
    return reinterpret_cast<void(*)()>(GetProcAddress(library.module(), name));
}
}
WindowsLibrary::WindowsLibrary(): module_(NULL)
{
    //
}

WindowsLibrary::WindowsLibrary(const QString& path)
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

WindowsLibrary::WindowsLibrary(WindowsLibrary&& rhs) noexcept : module_(NULL)
{
    std::swap(module_, rhs.module_);
#ifndef NDEBUG
    path_ = std::move(rhs.path_);
#endif
}

WindowsLibrary& WindowsLibrary::operator=(WindowsLibrary&& rhs) noexcept
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

WindowsLibrary::~WindowsLibrary() noexcept
{
    if(module_)
    {
        FreeLibrary(module_);
    }
}

HMODULE WindowsLibrary::module() const
{
    return module_;
}
}
