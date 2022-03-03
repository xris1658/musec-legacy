#include "DatabaseDAO.hpp"

#include "dao/AssetDirectoryDAO.hpp"
#include "dao/PluginDAO.hpp"
#include "dao/PluginDirectoryDAO.hpp"
#include "native/Native.hpp"

#include <QString>

#include <sqlite_modern_cpp.h>

namespace Musec::DAO
{
const QString& DatabasePath()
{
    static QString ret = QString(Musec::Native::DataDirectoryPath())
        .append(databaseNameWithBackslash);
    return ret;
}

sqlite::database connectDatabase(const QString& path)
{
    static sqlite::database database(path.toStdU16String());
    return database;
}

sqlite::database& AppDatabase()
{
    static auto ret = connectDatabase(DatabasePath());
    return ret;
}

void initDatabase()
{
    createPluginDirectoryTable();
    createPluginTable();
    createAssetDirectoryTable();
}
}
