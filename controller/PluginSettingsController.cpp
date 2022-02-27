#include "PluginSettingsController.hpp"
#include "base/PluginBase.hpp"
#include "dao/DatabaseDAO.hpp"
#include "dao/PluginDAO.hpp"
#include "dao/PluginDirectoryDAO.hpp"

#include <QDir>
#include <QList>

#include <queue>
#include <utility>

namespace Musec::Controller::PluginSettingsController
{
QStringList getPluginDirectoryList()
{
    return Musec::DAO::selectPluginDirectory();
}

void addPluginDirectory(const QString& path)
{
    Musec::DAO::addPluginDirectory(path);
}

void removePluginDirectory(const QString& path)
{
    Musec::DAO::removePluginDirectory(path);
}

/// 扫描插件
/// 请务必不要这个函数在 UI 线程上跑，否则扫描 SAK
/// 或 FabFilter 的插件时会有奇怪的错误出现，原因未知。
/// 就是这个玩意卡了我三天 :(
void scanPlugins()
{
    QStringList plugins;
    std::queue<QDir> pluginDirectories;
    QStringList nameFilters;
    nameFilters << "*.dll" << "*.vst3";
    Musec::DAO::removeAllPlugins();
    auto pluginDirectoryList = Musec::DAO::selectPluginDirectory();
    for(auto& item: pluginDirectoryList)
    {
        QDir dir(item);
        if(dir.exists())
        {
            pluginDirectories.emplace(std::move(dir));
        }
    }
    while(!pluginDirectories.empty())
    {
        auto size = pluginDirectories.size();
        for(decltype(size) i = 0; i < size; ++i)
        {
            auto& dir = pluginDirectories.front();
            auto fileList = dir.entryInfoList(nameFilters,
                                              QDir::Filter::Files | QDir::Filter::Hidden);
            auto fileListSize = fileList.size();
            if(fileListSize)
            {
                // 减少内存重新分配次数
                plugins.reserve(plugins.size() + fileListSize);
                for(decltype(fileListSize) i = 0; i < fileListSize; ++i)
                {
                    auto path = fileList[i].absoluteFilePath();
                    path.replace(QChar('/'), QChar('\\'));
                    plugins.append(path);
                }
            }
            auto dirList = dir.entryInfoList(
                    QDir::AllDirs
                    | QDir::Filter::Hidden
                    | QDir::Filter::NoDotAndDotDot);
            auto dirListSize = dirList.size();
            for(decltype(dirListSize) i = 0; i < dirListSize; ++i)
            {
                auto directory = dirList[i].absoluteFilePath();
                pluginDirectories.emplace(directory);
            }
            pluginDirectories.pop();
        }
    }
    for(auto& path: plugins)
    {
        auto pluginList = Musec::Base::scanSingleLibraryFile(path);
        for(auto& [uid, name, format, type]: pluginList)
        {
            Musec::DAO::insertPlugin(
                Musec::Base::PluginWriteInfo(
                    path,
                    uid,
                    name,
                    format,
                    type
                )
            );
        }
    }
}
}
