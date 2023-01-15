#include "PluginSettingsController.hpp"
#include "base/PluginBase.hpp"
#include "controller/ConfigController.hpp"
#include "dao/PluginDAO.hpp"
#include "dao/PluginDirectoryDAO.hpp"

#include <QDir>
#include <QList>

#include <queue>
#include <utility>
#include <vector>

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

void scanPlugins()
{
    QFlags<QDir::Filter> scanFileFlags = QDir::Filter::Files | QDir::Filter::Hidden;
    QStringList nameFilters;
    nameFilters << "*.vst3" << "*.clap";
    auto scanShortcut = Musec::Controller::ConfigController::appConfig()["musec"]["options"]["plugin"]["enable-shortcut"].as<bool>();
    QString lnk;
    if(scanShortcut)
    {
        scanFileFlags |= QDir::Filter::System;
        nameFilters << "*.lnk";
        lnk = nameFilters.back(); lnk.remove(0, 2);
    }
    std::vector<QString> plugins;
    std::queue<QDir> pluginDirectories;
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
            auto fileList = dir.entryInfoList(nameFilters, scanFileFlags);
            auto fileListSize = fileList.size();
            if(fileListSize)
            {
                for(decltype(fileListSize) i = 0; i < fileListSize; ++i)
                {
                    auto path = fileList[i].absoluteFilePath();
                    if(fileList[i].suffix() == lnk)
                    {
                        auto resolvedPath = fileList[i].symLinkTarget();
                        if(!resolvedPath.isEmpty())
                        {
                            resolvedPath.replace(QChar('/'), QChar('\\'));
                            if(!resolvedPath.isEmpty())
                            {
                                plugins.emplace_back(resolvedPath);
                            }
                        }
                    }
                    else
                    {
                        path.replace(QChar('/'), QChar('\\'));
                        plugins.emplace_back(path);
                    }
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

void setScanShortcut(bool value)
{
    using namespace Musec::Controller::ConfigController;
    appConfig()["musec"]["options"]["plugin"]["enable-shortcut"] = value;
}
}
