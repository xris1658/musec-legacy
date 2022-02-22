#include "controller/AssetDirectoryController.hpp"

#include "AppController.hpp"
#include "dao/DatabaseDAO.hpp"

#include <QDir>

#include <tuple>

namespace Musec::Controller::AssetDirectoryController
{
QList<Base::AssetDirectoryInfo> getAssetDirectory()
{
    QList<Base::AssetDirectoryInfo> ret =
            DAO::selectAllAssetDirectory(true);
    return ret;
}

void addAssetDirectory(const QString& directory)
{
    using namespace Model;
    // 根据目录位置得知目录名
    auto directory16 = directory.toStdU16String();
    auto name = directory.section('\\', -1);
    auto name16 = name.toStdU16String();
    // 添加素材目录
    DAO::addAssetDirectory(directory, name);
    // 视情况重新扫描此目录的素材文件
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
    }
    else
    {
        DAO::updateAssetDirectoryNameById(id, name);
    }
    auto& assetDirectoryList = AppAssetDirectoryList();
    assetDirectoryList.setList(decltype(assetDirectoryList.getList())());
    assetDirectoryList.setList(getAssetDirectory());
}

void removeAssetDirectory(int id)
{
    DAO::removeAssetDirectoryById(id);
    auto& assetDirectoryList = AppAssetDirectoryList();
    assetDirectoryList.setList(decltype(assetDirectoryList.getList())());
    assetDirectoryList.setList(getAssetDirectory());
}
}