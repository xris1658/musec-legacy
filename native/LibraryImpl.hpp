#ifndef MUSEC_NATIVE_LIBRARYIMPL
#define MUSEC_NATIVE_LIBRARYIMPL

#include "native/Library.hpp"
#include "native/Native.hpp"

#include <QString>

namespace Musec
{
namespace Native
{
class LibraryImpl
{
private:
#if(WIN32)
    using HandleType = HMODULE;
#else
#error Unknown HandleType
#endif
public:
    LibraryImpl();
    explicit LibraryImpl(const QString& path);
    LibraryImpl(const LibraryImpl&) = delete;
    LibraryImpl& operator=(const LibraryImpl&) = delete;
    LibraryImpl(LibraryImpl&& rhs) noexcept;
    LibraryImpl& operator=(LibraryImpl&& rhs) noexcept;
    ~LibraryImpl() noexcept;
public:
    ErrorCodeType errorCode() const;
    Musec::Native::Library::ExportType getExport(const char* name);
private:
    ErrorCodeType errorCode_ = 0;
    HandleType handle_;
};
}
}

#endif //MUSEC_NATIVE_LIBRARYIMPL
