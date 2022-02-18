#include "AssetDAO.hpp"
#include "dao/DatabaseDAO.hpp"

namespace Musec::DAO
{
namespace Impl
{
const char16_t* createAssetTableCommand()
{
    static char16_t ret[] =
        u"CREATE TABLE IF NOT EXISTS asset("
        "    `path` TEXT NOT NULL"
        "        CONSTRAINT `asset_pk`"
        "            PRIMARY KEY,"
        "    `asset_directory` INTEGER NOT NULL"
        "        REFERENCES `asset_directory`"
        "            ON UPDATE CASCADE ON DELETE CASCADE"
        ");";
    return ret;
}

const char16_t* selectAssetCommand()
{
    static char16_t ret[] =
        u"SELECT * FROM asset";
    return ret;
}

const char16_t* selectAssetByAssetDirectoryNameCommand()
{
    static char16_t ret[] =
        u"SELECT * FROM asset WHERE asset_directory = ?";
    return ret;
}
}
void createAssetTable()
{
    AppDatabase() << Impl::createAssetTableCommand();
}
}
