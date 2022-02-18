#ifndef MUSEC_NATIVE_NATIVE
#define MUSEC_NATIVE_NATIVE

#include <QString>

namespace Musec
{
namespace Native
{
// 漫游用户数据路径（用户文件夹\AppData\Roaming）
const QString& RoamingDirectoryPath();

// Musec 程序用户数据路径（用户文件夹\AppData\Roaming\Musec）
const QString& DataDirectoryPath();
}
}

#endif // MUSEC_NATIVE_NATIVE
