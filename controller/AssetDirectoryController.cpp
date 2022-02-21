#include "AssetDirectoryController.hpp"

#include "AppController.hpp"
#include "dao/DatabaseDAO.hpp"

#include <QDir>

#include <tuple>

namespace Musec::Controller
{
QList<Musec::Base::AssetDirectoryInfo> getAssetDirectory()
{
    QList<Musec::Base::AssetDirectoryInfo> ret =
        Musec::DAO::selectAllAssetDirectory(true);
    return ret;
}

void addAssetDirectory(const QString& directory)
{
    using namespace Musec::Model;
    // 根据目录位置得知目录名
    auto directory16 = directory.toStdU16String();
    auto name = directory.section('\\', -1);
    auto name16 = name.toStdU16String();
    // 添加素材目录
    Musec::DAO::addAssetDirectory(directory, name);
    // 视情况重新扫描此目录的素材文件
    int newId = Musec::DAO::getAssetDirectoryLastId();
    auto& assetDirectoryList = AppAssetDirectoryList();
    assetDirectoryList.append(AssetDirectoryListModel::Item(newId, directory, name));
}

void renameAssetDirectory(int id, const QString& name)
{
    if(name.length() == 0)
    {
        using namespace Musec::Model;
        QString directory =
        std::get<Musec::Base::AssetDirectoryInfoField::FieldDirectory>(
            Musec::DAO::selectAssetDirectoryById(id)
        );
        auto newName = directory.section('\\', -1);
        Musec::DAO::updateAssetDirectoryNameById(id, newName);
    }
    else
    {
        Musec::DAO::updateAssetDirectoryNameById(id, name);
    }
    auto& assetDirectoryList = AppAssetDirectoryList();
    assetDirectoryList.setList(decltype(assetDirectoryList.getList())());
    assetDirectoryList.setList(getAssetDirectory());
}

void removeAssetDirectory(int id)
{
    Musec::DAO::removeAssetDirectoryById(id);
    auto& assetDirectoryList = AppAssetDirectoryList();
    assetDirectoryList.setList(decltype(assetDirectoryList.getList())());
    assetDirectoryList.setList(getAssetDirectory());
}
}
