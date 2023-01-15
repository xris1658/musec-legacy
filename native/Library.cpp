#include "Library.hpp"

#include "native/LibraryImpl.hpp"

namespace Musec::Native
{
Library::Library(): pImpl_()
{
}

Library::Library(const QString& path):
    pImpl_(std::make_unique<Musec::Native::LibraryImpl>(path))
{
}

Library::Library(Library&& rhs) noexcept: pImpl_(std::move(rhs.pImpl_))
{

}

Library& Library::operator=(Library&& rhs) noexcept
{
    pImpl_ = std::move(rhs.pImpl_);
    return *this;
}

Library::~Library() noexcept
{
}

ErrorCodeType Library::errorCode() const
{
    return pImpl_? pImpl_->errorCode(): 0;
}

Library::ExportType Library::getExportImpl(const char* name)
{
    return pImpl_? pImpl_->getExport(name): nullptr;
}
}