#include "AppController.hpp"

#include "base/Constants.hpp"
#include "controller/AssetDirectoryController.hpp"
#include "controller/ConfigController.hpp"
#include "controller/LoggingController.hpp"
#include "controller/PluginController.hpp"
#include "controller/PluginSettingsController.hpp"
#include "dao/PluginDirectoryDAO.hpp"
#include "event/EventBase.hpp"
#include "native/Native.hpp"
#include "ui/MessageDialog.hpp"
#include "ui/Render.hpp"
#include "ui/UI.hpp"

#include <QDir>
#include <QFile>
#include <QApplication>

#include <string>
#include <tuple>
#include <array>

namespace Musec::Controller
{
void loadOrInitConfig()
{
    using namespace Musec::Event;
    auto strings = Musec::UI::strings;
    if(Musec::Controller::findAppData())
    {
        Musec::Controller::loadAppData();
    }
    else
    {
        Musec::Controller::initAppData();
    }
}

void initApplication(Musec::Event::SplashScreen* splashScreen)
{
    using namespace Musec::UI;
    auto& appConfig = Musec::Controller::ConfigController::appConfig();
    splashScreen->setBootText("searchingAudioDeviceText");
    decltype(Musec::Audio::Driver::enumerateDrivers()) driverList;
    try
    {
        driverList = Musec::Audio::Driver::enumerateDrivers();
        AppASIODriverList().setList(driverList);
    }
    catch(std::exception&)
    {
        Musec::UI::MessageDialog::messageDialog(
           strings->property("enumeratingASIODriverErrorText").toString(),
           Musec::Base::ProductName,
           Musec::UI::MessageDialog::IconType::Error
        );
    }
    splashScreen->setBootText("searchingASIODriverText");
    Musec::Audio::Driver::AppASIODriver();
    auto driverCLSID = QString::fromStdString(
        appConfig["musec"]["options"]["audio-hardware"]["driver-id"]
            .as<std::string>()
    );
    if(driverList.empty())
    {
        Musec::UI::MessageDialog::messageDialog(
           strings->property("noASIODriverFoundText").toString(),
           Musec::Base::ProductName,
           Musec::UI::MessageDialog::IconType::Warning
        );
    }
    else
    {
        using namespace Musec::Audio::Driver;
        for(auto& item: driverList)
        {
            if(std::get<ASIODriverField::CLSIDField>(item) == driverCLSID)
            {
                splashScreen->setBootText("loadingASIODriverText");
                try
                {
                    AppASIODriver() = ASIODriver(item);
                }
                catch(std::runtime_error& exception)
                {
                   Musec::UI::MessageDialog::messageDialog(
                       strings->property("loadASIODriverErrorText").toString(),
                       Musec::Base::ProductName,
                       Musec::UI::MessageDialog::IconType::Warning);
                   AppASIODriver() = ASIODriver();
                }
                break;
            }
        }
    }
    splashScreen->setBootText("openingMainWindowText");
}



bool findAppData()
{
    QFile file(Musec::DAO::DatabasePath());
    return file.exists();
}

void initAppData()
{
    QDir roaming(Musec::Native::roamingAppDataFolder());
    roaming.mkdir(Musec::Base::ProductName);
    QDir dir(Musec::Native::dataDirectoryPath());
    Musec::DAO::initDatabase();
    Musec::Controller::ConfigController::createAppConfig();
    Musec::Controller::ConfigController::saveAppConfig();
    auto& list = Musec::Base::defaultPluginDirectoryList();
    for(auto& item: list)
    {
        Musec::DAO::addPluginDirectory(item);
    }
}

void loadAppData()
{
    auto& appConfig = Musec::Controller::ConfigController::appConfig();
    refreshPluginList(false);
    loadAssetDirectoryList();
}

Musec::Model::PluginListModel& AppPluginList()
{
    static Musec::Model::PluginListModel ret;
    return ret;
}

Musec::Model::PluginListModel& AppMidiEffectList()
{
    static Musec::Model::PluginListModel ret;
    return ret;
}

Musec::Model::PluginListModel& AppInstrumentList()
{
    static Musec::Model::PluginListModel ret;
    return ret;
}

Musec::Model::PluginListModel& AppAudioEffectList()
{
    static Musec::Model::PluginListModel ret;
    return ret;
}

Musec::Model::ASIODriverListModel& AppASIODriverList()
{
    static Musec::Model::ASIODriverListModel ret;
    return ret;
}

Musec::Model::AssetDirectoryListModel& AppAssetDirectoryList()
{
    static Musec::Model::AssetDirectoryListModel ret;
    return ret;
}

Musec::Model::TranslationFileModel& AppTranslationFileList()
{
    static Musec::Model::TranslationFileModel ret;
    return ret;
}

void refreshPluginList(bool rescan)
{
    decltype(AppPluginList().getList()) emptyList;
    AppPluginList().setList(emptyList);
    AppMidiEffectList().setList(emptyList);
    AppInstrumentList().setList(emptyList);
    AppAudioEffectList().setList(emptyList);
    auto setPluginLists = []()
    {
        using namespace Musec::Model;
        auto list = PluginController::getAllPlugins();
        auto pluginTypeList = [](int pluginType) -> PluginListModel&
        {
            return pluginType == 1? AppMidiEffectList():
                   pluginType == 2? AppInstrumentList():
                                    AppAudioEffectList();
        };
        std::array<PluginListModel::List, 3> typePluginList =
        {
            PluginListModel::List(),
            PluginListModel::List(),
            PluginListModel::List()
        };
        for(auto& item: list)
        {
            using namespace Musec::Base;
            auto& pluginType =
                std::get<PluginListModel::RoleNames::TypeRole - Qt::UserRole>(item);
            if(pluginType != PluginType::TypeUnknown)
            {
                typePluginList[pluginType - 1].append(item);
            }
        }
        for(int i = 1; i <= 3; ++i)
        {
            pluginTypeList(i).setList(typePluginList[i - 1]);
        }
        AppPluginList().setList(list);
    };
    if(rescan)
    {
        auto scanPluginLambda = [&setPluginLists]()
        {
            using namespace Musec::Event;
            using namespace Musec::Model;
            Musec::Controller::PluginSettingsController::scanPlugins();
            // auto list = Musec::Controller::getAllPlugins();
            setPluginLists();
            eventHandler->scanPluginComplete();
        };
        // scan plugins on a new thread
        // scanPluginLambda will be copied.
        std::thread(scanPluginLambda).detach();
    }
    else
    {
        setPluginLists();
    }
}

void loadAssetDirectoryList()
{
    AppAssetDirectoryList().setList(AssetDirectoryController::getAssetDirectory());
}

void openSpecialCharacterInput()
{
    Musec::Native::openSpecialCharacterInput();
}

}
