#include "AppController.hpp"

#include "controller/AssetDirectoryController.hpp"
#include "controller/ConfigController.hpp"
#include "controller/PluginController.hpp"
#include "controller/PluginSettingsController.hpp"
#include "dao/PluginDirectoryDAO.hpp"
#include "event/EventBase.hpp"
#include "native/Native.hpp"
#include "ui/MessageDialog.hpp"
#include "ui/Render.hpp"

#include <QDir>
#include <QFile>
#include <QApplication>

#include <string>
#include <tuple>
#include <array>

namespace Musec::Controller
{
Musec::DAO::LoggingDAO::LoggerType& createLogger()
{
    return Musec::DAO::LoggingDAO::createLogger();
}

void initApplication(Musec::Event::SplashScreen* splashScreen)
{
    using namespace Musec::Event;
    splashScreen->setBootText("正在启动...");
    if(Musec::Controller::findAppData())
    {
        splashScreen->setBootText("正在加载应用设置...");
        Musec::Controller::loadAppData();
    }
    else
    {
        splashScreen->setBootText("正在初始化应用设置...");
        Musec::Controller::initAppData();
        // 在这里添加打开初次设置窗口的操作
    }
    auto systemRender =
        Musec::Controller::ConfigController::appConfig()
        ["musec"]["options"]["general"]["system-render"]
        .as<bool>();
    if(systemRender)
    {
        Musec::UI::Render::setSystemRender();
    }
    auto& appConfig = Musec::Controller::ConfigController::appConfig();
    splashScreen->setBootText("正在寻找音频设备...");
    // 加载 ASIO 驱动列表
    auto driverList = Musec::Audio::Driver::enumerateDrivers();
    AppASIODriverList().setList(driverList);
    // 加载 ASIO 驱动
    splashScreen->setBootText("正在寻找 ASIO 驱动程序...");
    Musec::Audio::Driver::AppASIODriver();
    auto driverCLSID = QString::fromStdString(
        appConfig["musec"]["options"]["audio-hardware"]["driver-id"]
            .as<std::string>()
    );
    if(driverList.empty())
    {
        // 没有找到 ASIO 驱动，程序将以音频引擎关闭的状态运行
        Musec::Audio::Driver::AppASIODriver();
        Musec::UI::MessageDialog::messageDialog("在此电脑上找不到 ASIO 驱动程序。程序将以未加载音频驱动的方式运行。",
                                                "Musec",
                                                Musec::UI::MessageDialog::IconType::Warning);
    }
    else
    {
        using namespace Musec::Audio::Driver;
        for(auto& item: driverList)
        {
            if(std::get<ASIODriverField::CLSIDField>(item) == driverCLSID)
            {
                splashScreen->setBootText("正在加载 ASIO 驱动程序...");
                try
                {
                    AppASIODriver() = ASIODriver(item);
                }
                catch(std::runtime_error& exception)
                {
                    Musec::UI::MessageDialog::messageDialog("无法加载 ASIO 驱动程序。程序将以未加载音频驱动的方式运行。",
                                                         "Musec",
                                                         Musec::UI::MessageDialog::IconType::Warning);
                    AppASIODriver() = ASIODriver();
                }
                break;
            }
        }
    }
    splashScreen->setBootText("正在打开主界面...");
}



bool findAppData()
{
    QFile file(Musec::DAO::DatabasePath());
    return file.exists();
}

void initAppData()
{
    QDir roaming(Musec::Native::RoamingDirectoryPath());
    roaming.mkdir("Musec");
    QDir dir(Musec::Native::DataDirectoryPath());
    dir.refresh();
    if(!dir.exists())
    {
        // 抛出无法创建文件夹的异常
    }
    // 初始化数据库
    Musec::DAO::initDatabase();
    // 创建配置文件（未完成）
    // 完成此处内容之前，打开程序会使程序崩溃。
    Musec::Controller::ConfigController::createAppConfig();
    Musec::Controller::ConfigController::saveAppConfig();
    // 添加一些默认的插件目录（e.g. C:\Program Flies\VstPlugins）
    auto& list = Musec::Base::defaultPluginDirectoryList();
    for(auto& item: list)
    {
        Musec::DAO::addPluginDirectory(item);
    }
    // 添加自带素材目录（暂无）
}

void loadAppData()
{
    // 读取设置文件
    auto& appConfig = Musec::Controller::ConfigController::appConfig();
    // 视情况重新扫描插件
    refreshPluginList(false);
    loadAssetDirectoryList();
    // 视情况重新扫描素材
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
            typePluginList[pluginType - 1].append(item);
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
        // 在新线程上扫描插件（参见 scanPlugins() 的注释）
        // join 或 detach 均可，因为 scanPluginLambda 会被复制（参见）
        // 初始化对象时获取的 scanPluginLambda 按 const 左值引用传递？
        std::thread thread(scanPluginLambda);
        thread.detach();
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
