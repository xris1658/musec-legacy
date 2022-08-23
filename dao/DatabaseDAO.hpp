#ifndef MUSEC_DAO_DATABASEDAO
#define MUSEC_DAO_DATABASEDAO

#include <QString>

#include <sqlite_modern_cpp.h>

namespace Musec
{
namespace DAO
{
constexpr auto databaseNameWithBackslash = "\\Musec.db";
// <User folder>\AppData\Roaming\Musec\Musec.db
const QString& DatabasePath();

sqlite::database connectDatabase(const QString&);

sqlite::database& AppDatabase();

void initDatabase();
}
}

#endif // MUSEC_DAO_DATABASEDAO
