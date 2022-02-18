#include "AppController.hpp"

#include "controller/PluginSettingsController.hpp"
#include "controller/AssetDirectoryController.hpp"
#include "controller/PluginController.hpp"
#include "audio/driver/ASIODriver.hpp"
#include "audio/driver/ASIOCallback.hpp"
#include "dao/DatabaseDAO.hpp"
#include "dao/AssetDirectoryDAO.hpp"
#include "dao/ConfigDAO.hpp"
#include "dao/PluginDAO.hpp"
#include "dao/PluginDirectoryDAO.hpp"
#include "event/EventBase.hpp"
#include "native/Native.hpp"

#include <QDir>
#include <QString>
#include <QApplication>
#include <QQuickView>

#include <string>
#include <tuple>
#include <array>
#include <ui/UI.hpp>

namespace Musec::Controller
{
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
        Musec::Controller::initAppData();
        // 在这里添加打开初次设置窗口的操作
    }
    auto appConfig = Musec::DAO::loadAppConfig(Musec::DAO::ConfigFilePath());
    splashScreen->setBootText("正在寻找音频设备...");
    // 加载 ASIO 驱动列表
    auto driverList = Musec::Audio::Driver::enumerateDrivers();
    AppASIODriverList().setList(driverList);
    // 加载 ASIO 驱动
    auto driverCLSID = QString::fromStdString(
        appConfig["musec"]["options"]["audio-hardware"]["driver-id"]
            .as<std::string>()
    );
    if(driverList.empty())
    {
        // 没有找到 ASIO 驱动，程序将以音频引擎关闭的状态运行
    }
    else
    {
        using namespace Musec::Audio::Driver;
        for(auto& item: driverList)
        {
            if(std::get<ASIODriverField::CLSIDField>(item) == driverCLSID)
            {
                splashScreen->setBootText("正在加载 ASIO 驱动程序...");
                AppASIODriver() = ASIODriver(item);
                break;
            }
        }
        if(!AppASIODriver())
        {
            splashScreen->setBootText("正在加载 ASIO 驱动程序...");
            // 没有找到设置中的 ASIO 驱动，直接用表中的第一个
            // （可能会导致一些潜在问题的出现，不过暂时不管）
            AppASIODriver() = ASIODriver(driverList[0]);
        }
    }
    splashScreen->setBootText("正在打开主界面...");
}

bool findAppData()
{
    auto musecDataDirectory = Musec::Native::DataDirectoryPath();
    QDir dir(musecDataDirectory);
    return dir.exists();
}

void initAppData()
{
    using namespace Musec::DAO;
    auto roamingDirectory = Musec::Native::RoamingDirectoryPath();
    QDir roaming(roamingDirectory);
    roaming.mkdir("Musec");
    auto musecDataDirectory = Musec::Native::DataDirectoryPath();
    QDir dir(musecDataDirectory);
    dir.refresh();
    if(!dir.exists())
    {
        // 抛出无法创建文件夹的异常
    }
    // 初始化数据库
    initDatabase();
    // 创建配置文件（未完成）
    // 完成此处内容之前，打开程序会使程序崩溃。
    saveAppConfig(createAppConfig(), Musec::DAO::ConfigFilePath());
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
    using namespace Musec::DAO;
    // 读取设置文件
    auto appConfig = loadAppConfig(ConfigFilePath());
    // 视情况重新扫描插件
    refreshPluginList(false);
    loadAssetDirectoryList();
    // 视情况重新扫描素材
}

YAML::Node createAppConfig()
{
    YAML::Node configFileNode;
    configFileNode["musec"]["options"]["general"]["language"] = "zh-cn";
    configFileNode["musec"]["options"]["general"]["theme"] = "dark";
    configFileNode["musec"]["options"]["general"]["scale"] = 100;
    configFileNode["musec"]["options"]["general"]["touch"] = true;
    configFileNode["musec"]["options"]["general"]["single-instance"] = true;
    configFileNode["musec"]["options"]["audio-hardware"]["driver-type"] = "ASIO";
    configFileNode["musec"]["options"]["audio-hardware"]["driver-id"] =
        "{232685C6-6548-49D8-846D-4141A3EF7560}"; // ASIO4ALL
    configFileNode["musec"]["options"]["audio-hardware"]["buffer-size"] = 512;
    configFileNode["musec"]["options"]["audio-hardware"]["sample-rate"] = 44100;
    configFileNode["musec"]["options"]["plugin"]["enable-shortcuts"] = true;
    configFileNode["musec"]["options"]["plugin"]["enable-32-bit"] = false;
    return configFileNode;
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
        auto list = Musec::Controller::getAllPlugins();
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
            Musec::Controller::scanPlugins();
            auto list = Musec::Controller::getAllPlugins();
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
    AppAssetDirectoryList().setList(getAssetDirectory());
}

void loadASIODriver()
{
    using namespace Musec::Audio::Driver;
    using namespace Musec::DAO;
    using namespace Musec::UI;
    auto& driver = AppASIODriver();
    auto hWnd = reinterpret_cast<HWND>(mainWindow->winId());
    driver->init(hWnd);
    auto appConfig = loadAppConfig(Musec::DAO::ConfigFilePath());
    auto sampleRate = appConfig["musec"]["options"]["audio-hardware"]["sample-rate"].as<double>();
    auto bufferSize = appConfig["musec"]["options"]["audio-hardware"]["buffer-size"].as<int>();
    auto canSampleRate = driver->canSampleRate(sampleRate);
    if(canSampleRate == ASE_OK || canSampleRate == ASE_SUCCESS)
    {
        driver->setSampleRate(sampleRate);
    }
    else
    {
        driver->setSampleRate(44100.0);
    }
    auto info = getASIODriverStreamInfo(driver);
    constexpr int inputBufferCount = 64;
    constexpr int outputBufferCount = 64;
    auto& bufferInfo = getASIOBufferInfoList();
    for(int i = 0; i < info.inputChannelCount; ++i)
    {
        bufferInfo[i].isInput = ASIOTrue;
        bufferInfo[i].channelNum = i;
        bufferInfo[i].buffers[0] = bufferInfo[i].buffers[1] = nullptr;
    }
    for(int i = info.inputChannelCount; i < info.inputChannelCount + info.outputChannelCount; ++i)
    {
        bufferInfo[i].isInput = ASIOFalse;
        bufferInfo[i].channelNum = i - info.inputChannelCount;
        bufferInfo[i].buffers[0] = bufferInfo[i].buffers[1] = nullptr;
    }
    //
    auto createBuffersResult = driver->createBuffers(bufferInfo.data(),
                          inputBufferCount + outputBufferCount,
                          info.preferredBufferSize,
                          &getCallbacks());
    driver->start();
}

QString getASIODriver()
{
    auto appConfig = Musec::DAO::loadAppConfig(Musec::DAO::ConfigFilePath());
    return QString::fromStdString(
        appConfig["musec"]["options"]["audio-hardware"]["driver-id"].as<std::string>()
    );
}

void setASIODriver(const QString& clsid)
{
    using namespace Musec::Audio::Driver;
    auto driverList = AppASIODriverList().getList();
    for(auto& item: driverList)
    {
        auto& itemCLSID = std::get<ASIODriverField::CLSIDField>(item);
        if(itemCLSID.compare(clsid, Qt::CaseSensitivity::CaseInsensitive) == 0)
        {
            AppASIODriver() = ASIODriver(item);
            auto appConfig = Musec::DAO::loadAppConfig(Musec::DAO::ConfigFilePath());
            appConfig["musec"]["options"]["audio-hardware"]["driver-id"] =
                clsid.toStdString();
            Musec::DAO::saveAppConfig(appConfig, Musec::DAO::ConfigFilePath());
            break;
        }
    }
}

void openASIODriverControlPanel()
{
    using namespace Musec::Audio::Driver;
    AppASIODriver()->controlPanel();
}

void unloadASIODriver()
{
    using namespace Musec::Audio::Driver;
    AppASIODriver() = ASIODriver();
}

void updateCurrentASIODriverInfo()
{
    using namespace Musec::Audio::Driver;
    using namespace Musec::UI;
    if(optionsWindow)
    {
        // 获取驱动信息
        auto driverStreamInfo = getASIODriverStreamInfo(AppASIODriver());
        optionsWindow->setProperty("bufferSize",
            QVariant::fromValue<int>(driverStreamInfo.preferredBufferSize));
        optionsWindow->setProperty("inputLatencyInSamples",
            QVariant::fromValue<int>(driverStreamInfo.inputLatencyInSamples));
        optionsWindow->setProperty("outputLatencyInSamples",
            QVariant::fromValue<int>(driverStreamInfo.outputLatencyInSamples));
        optionsWindow->setProperty("sampleRate",
            QVariant::fromValue<double>(driverStreamInfo.sampleRate));
    }
}

}
