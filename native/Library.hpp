#ifndef MUSEC_NATIVE_LIBRARY
#define MUSEC_NATIVE_LIBRARY

#include "native/Native.hpp"

#include <QString>

#include <memory>

namespace Musec
{
namespace Native
{
class LibraryImpl;


class Library
{
public:
#if(WIN32)
    using ExportType = FARPROC;
#else
#error Unknown ExportType
#endif
public:
    Library();
    explicit Library(const QString& path);
    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;
    Library(Library&& rhs) noexcept;
    Library& operator=(Library&& rhs) noexcept;
    ~Library() noexcept;
private:
    ExportType getExportImpl(const char* name);
public:
    ErrorCodeType errorCode() const;
    template<typename T>
    T getExport(const char* name)
    {
        return (T)getExportImpl(name);
    }
public:
    std::unique_ptr<LibraryImpl> pImpl_;
};
}
}

#endif //MUSEC_NATIVE_LIBRARY
