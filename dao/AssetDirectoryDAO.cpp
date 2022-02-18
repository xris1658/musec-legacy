#include "AssetDirectoryDAO.hpp"

#include "dao/DatabaseDAO.hpp"

#include <string>

namespace Musec::DAO
{
namespace Impl
{
const char16_t* createAssetDirectoryTableCommand()
{
    static char16_t ret[] =
        u"CREATE TABLE IF NOT EXISTS asset_directory("
        "   `id` INTEGER NOT NULL"
        "       CONSTRAINT id_pk"
        "           PRIMARY KEY AUTOINCREMENT,"
        "   `path` TEXT NOT NULL,"
        "   `name` TEXT NOT NULL"
        ");";
    return ret;
}

const char16_t* addAssetDirectoryCommand()
{
    static char16_t ret[] =
        u"INSERT INTO asset_directory (path, name)"
        "VALUES(?, ?)";
    return ret;
}

const char16_t* getAssetDirectoryLastIdCommand()
{
    static char16_t ret[] =
        u"SELECT seq FROM sqlite_sequence WHERE name = \'asset_directory\'";
    return ret;
}

const char16_t* selectAllAssetDirectoryCommand()
{
    static char16_t ret[] =
        u"SELECT * FROM asset_directory";
    return ret;
}

const char16_t* getAllAssetDirectoryCountCommand()
{
    static char16_t ret[] =
        u"SELECT COUNT(*) FROM asset_directory";
    return ret;
}

const char16_t* selectAssetDirectoryByIdCommand()
{
    static char16_t ret[] =
        u"SELECT * FROM asset_directory WHERE id = ?";
    return ret;
}

const char16_t* updateAssetDirectoryNameByIdCommand()
{
    static char16_t ret[] =
        u"UPDATE asset_directory SET name = ? WHERE id = ?";
    return ret;
}

const char16_t* removeAssetDirectoryByIdCommand()
{
    static char16_t ret[] =
        u"DELETE FROM asset_directory WHERE id = ?";
    return ret;
}
}

void createAssetDirectoryTable()
{
    AppDatabase() << Impl::createAssetDirectoryTableCommand();
}

void addAssetDirectory(const QString& path, const QString& name)
{
    AppDatabase() << Impl::addAssetDirectoryCommand()
                  << path.toStdU16String() << name.toStdU16String();
}

int getAssetDirectoryLastId()
{
    int ret;
    AppDatabase() << Impl::getAssetDirectoryLastIdCommand() >> ret;
    return ret;
}

QList<Musec::Base::AssetDirectoryInfo> selectAllAssetDirectory(bool reserve)
{
    QList<Musec::Base::AssetDirectoryInfo> ret;
    if(reserve)
    {
        ret.reserve(getAssetDirectoryCount());
    }
    AppDatabase() << Impl::selectAllAssetDirectoryCommand()
                  >> [&ret](int id,
                            const std::u16string& directory,
                            const std::u16string& name)
    {
        ret.append(
            Musec::Base::AssetDirectoryInfo(
                id,
                QString::fromStdU16String(directory),
                QString::fromStdU16String(name)
            )
        );
    };
    return ret;
}

int getAssetDirectoryCount()
{
    int ret;
    AppDatabase() << Impl::getAllAssetDirectoryCountCommand() >> ret;
    return ret;
}

Musec::Base::AssetDirectoryInfo selectAssetDirectoryById(int id)
{
    Musec::Base::AssetDirectoryInfo ret;
    AppDatabase() << Impl::selectAssetDirectoryByIdCommand() << id
                  >> [&ret](int id,
                     const std::u16string& directory16,
                     const std::u16string& name)
    {
        ret = Musec::Base::AssetDirectoryInfo(id,
            QString::fromStdU16String(directory16),
            QString::fromStdU16String(name)
        );
    };
    return ret;
}

void updateAssetDirectoryNameById(int id, const QString& name)
{
    AppDatabase() << Impl::updateAssetDirectoryNameByIdCommand()
                  << name.toStdU16String() << id;
}

void removeAssetDirectoryById(int id)
{
    AppDatabase() << Impl::removeAssetDirectoryByIdCommand() << id;
}
}
