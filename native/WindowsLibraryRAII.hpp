#ifndef MUSEC_NATIVE_WINDOWSLIBRARYRAII
#define MUSEC_NATIVE_WINDOWSLIBRARYRAII

#include <QString>

#include <Windows.h>

namespace Musec
{
namespace Native
{
class WindowsLibraryRAII
{
public:
    using ExceptionType = decltype(GetLastError());
public:
    WindowsLibraryRAII();
    explicit WindowsLibraryRAII(const QString& path);
    WindowsLibraryRAII(const WindowsLibraryRAII&) = delete;
    WindowsLibraryRAII& operator=(const WindowsLibraryRAII&) = delete;
    WindowsLibraryRAII(WindowsLibraryRAII&&);
    WindowsLibraryRAII& operator=(WindowsLibraryRAII&&);
    ~WindowsLibraryRAII() noexcept;
public:
    HMODULE module() const;
private:
    HMODULE module_;
};

template<typename ReturnType>
ReturnType getExport(const WindowsLibraryRAII& library, const char* name)
{
    return reinterpret_cast<ReturnType>(GetProcAddress(library.module(), name));
}
}
}

#endif // MUSEC_NATIVE_WINDOWSLIBRARYRAII
