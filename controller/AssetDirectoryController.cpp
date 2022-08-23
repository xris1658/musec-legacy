#include "AssetDirectoryController.hpp"

#include "dao/DatabaseDAO.hpp"

#include <QDir>

#include <tuple>

namespace Musec::Controller::AssetDirectoryController
{
QList<Base::AssetDirectoryInfo> getAssetDirectory()
{
    QList<Base::AssetDirectoryInfo> ret =
        Musec::DAO::selectAllAssetDirectory(true);
    return ret;
}

void addAssetDirectory(const QString& directory)
{
    using namespace Model;
    auto directory16 = directory.toStdU16String();
    auto name = directory.section('\\', -1);
    // Root of Windows disk
    if(name.length() == 0)
    {
        name = directory.section('\\', -2);
    }
    auto name16 = name.toStdU16String();
    DAO::addAssetDirectory(directory, name);
    int newId = DAO::getAssetDirectoryLastId();
    auto& assetDirectoryList = AppAssetDirectoryList();
    assetDirectoryList.append(AssetDirectoryListModel::Item(newId, directory, name));
}

void renameAssetDirectory(int id, const QString& name)
{
    if(name.length() == 0)
    {
        using namespace Model;
        QString directory =
                std::get<Base::FieldDirectory>(
                        DAO::selectAssetDirectoryById(id)
                );
        auto newName = directory.section('\\', -1);
        DAO::updateAssetDirectoryNameById(id, newName);
        AppAssetDirectoryList().rename(id, newName);
    }
    else
    {
        DAO::updateAssetDirectoryNameById(id, name);
        AppAssetDirectoryList().rename(id, name);
    }
}

void removeAssetDirectory(int id)
{
    DAO::removeAssetDirectoryById(id);
    AppAssetDirectoryList().remove(id);
    // auto& assetDirectoryList = AppAssetDirectoryList();
    // assetDirectoryList.setList(decltype(assetDirectoryList.getList())());
    // assetDirectoryList.setList(getAssetDirectory());
}
}
