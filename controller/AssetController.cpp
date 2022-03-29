#include "AssetController.hpp"

namespace Musec::Controller::AssetController
{

Musec::Model::FolderListModel::List getFolderInDirectory(const QString& directory)
{
    QDir dir(directory);
    Musec::Model::FileListModel::List ret;
    if(dir.exists())
    {
        auto entryInfoList = dir.entryInfoList(QDir::Filter::AllDirs
                                             | QDir::Filter::Hidden
                                             | QDir::Filter::NoDotAndDotDot);
        auto size = entryInfoList.size();
        ret.reserve(size);
        for(decltype(size) i = 0; i < size; ++i)
        {
            auto folder = entryInfoList[i];
            ret.append(std::tuple(folder.fileName(), folder.absoluteFilePath()));
        }
    }
    return ret;
}

Musec::Model::FileListModel::List getFileInDirectory(const QString& directory)
{
    QDir dir(directory);
    Musec::Model::FileListModel::List ret;
    if(dir.exists())
    {
        auto entryInfoList = dir.entryInfoList(QDir::Filter::Files
                                             | QDir::Filter::Hidden);
        auto size = entryInfoList.size();
        ret.reserve(size);
        for(decltype(size) i = 0; i < size; ++i)
        {
            auto folder = entryInfoList[i];
            ret.append(std::tuple(folder.fileName(), folder.absoluteFilePath()));
        }
    }
    return ret;
}
}
