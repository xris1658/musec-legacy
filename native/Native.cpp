#include "Native.hpp"

#include <ShlObj_core.h>

#include <cstring>
#include <cwchar>
#include <memory>

namespace Musec
{
namespace Native
{
const QString& RoamingDirectoryPath()
{
    static wchar_t path[MAX_PATH] = {0};
    auto getFolderResult = SHGetFolderPathW(nullptr, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, path);
    if(getFolderResult != S_OK)
    {
        // TODO: 异常处理
    }
    static auto ret = QString::fromWCharArray(path);
    return ret;
}

const QString& DataDirectoryPath()
{
    static QString ret = QString(RoamingDirectoryPath()).append("\\Musec");
    return ret;
}
} }
