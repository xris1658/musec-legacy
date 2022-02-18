#ifndef MUSEC_DAO_DATABASEDAO
#define MUSEC_DAO_DATABASEDAO

#include <QString>

// SQLite
#include <sqlite_modern_cpp.h>

namespace Musec
{
namespace DAO
{
constexpr auto databaseNameWithBackslash = "\\Musec.db";
// 数据库路径（用户文件夹\AppData\Roaming\Musec\Musec.db）
const QString& DatabasePath();

// 连接数据库，若文件不存在则新建并连接数据库。（UTF-16）
sqlite::database connectDatabase(const QString&);

// 应用程序使用的数据库
sqlite::database& AppDatabase();

// 初始化数据库
void initDatabase();
}
}

#endif // MUSEC_DAO_DATABASEDAO
