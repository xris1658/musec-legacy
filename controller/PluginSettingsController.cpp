#include "PluginSettingsController.hpp"

#include "base/PluginBase.hpp"
#include "controller/ConfigController.hpp"
#include "dao/PluginDAO.hpp"
#include "dao/PluginDirectoryDAO.hpp"

#include <QDir>
#include <QList>

#include <deque>
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
        lnk = "lnk";
    }
    std::deque<QString> libraryPaths;
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
            for(auto& fileInfo: fileList)
            {
                auto path = fileInfo.absoluteFilePath();
                if(fileInfo.suffix() == lnk)
                {
                    auto resolvedPath = fileInfo.symLinkTarget();
                    if(!resolvedPath.isEmpty())
                    {
                        resolvedPath.replace(QChar('/'), QChar('\\'));
                        libraryPaths.emplace_back(resolvedPath);
                    }
                }
                else
                {
                    path.replace(QChar('/'), QChar('\\'));
                    libraryPaths.emplace_back(path);
                }
            }
            auto dirList = dir.entryInfoList(
                    QDir::AllDirs
                    | QDir::Filter::Hidden
                    | QDir::Filter::NoDotAndDotDot);
            auto dirListSize = dirList.size();
            for(auto& dirInfo: dirList)
            {
                pluginDirectories.emplace(dirInfo.absoluteFilePath());
            }
            pluginDirectories.pop();
        }
    }
    for(auto& path: libraryPaths)
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
