#ifndef MUSEC_NATIVE_WINDOWSLIBRARY
#define MUSEC_NATIVE_WINDOWSLIBRARY

#include <QString>

#include <Windows.h>

namespace Musec
{
namespace Native
{
class WindowsLibrary;
namespace Impl
{
using ExportType = void(*)();
ExportType getExport(const WindowsLibrary& library, const char* name);
}
class WindowsLibrary
{
public:
    using ExceptionType = decltype(GetLastError());
public:
    WindowsLibrary();
    explicit WindowsLibrary(const QString& path);
    WindowsLibrary(const WindowsLibrary&) = delete;
    WindowsLibrary& operator=(const WindowsLibrary&) = delete;
    WindowsLibrary(WindowsLibrary&&) noexcept;
    WindowsLibrary& operator=(WindowsLibrary&&) noexcept;
    virtual ~WindowsLibrary() noexcept;
public:
    HMODULE module() const;
private:
    HMODULE module_;
#ifndef NDEBUG
    QString path_;
#endif
};

template<typename ReturnType>
ReturnType getExport(const WindowsLibrary& library, const char* name)
{
    return reinterpret_cast<ReturnType>(Impl::getExport(library, name));
}
}
}

#endif // MUSEC_NATIVE_WINDOWSLIBRARY
