#include "EventHandler.hpp"

#include "audio/driver/ASIODriver.hpp"
#include "controller/AppController.hpp"
#include "controller/AssetDirectoryController.hpp"
#include "controller/PluginSettingsController.hpp"
#include "event/EventBase.hpp"
#include "ui/UI.hpp"

namespace Musec::Event
{
EventHandler::EventHandler(QObject* eventBridge, QObject* parent): QObject(parent)
{
    using namespace Musec::Event;
    using namespace Musec::UI;
    using Musec::Event::MainWindow;
    // QML -> C++
    QObject::connect(eventBridge, SIGNAL(optionsWindowOpened()),
                     this,        SLOT(onOptionsWindowOpened()));
    QObject::connect(eventBridge, SIGNAL(optionsWindowClosed()),
                     this,        SLOT(onOptionsWindowClosed()));
    QObject::connect(eventBridge, SIGNAL(pluginDirectoryAdded(QString)),
                     this,        SLOT(onPluginDirectoryAdded(QString)));
    QObject::connect(eventBridge, SIGNAL(pluginDirectoryRemoved(QString)),
                     this,        SLOT(onPluginDirectoryRemoved(QString)));
    QObject::connect(eventBridge, SIGNAL(scanPluginButtonClicked()),
                     this,        SLOT(onScanPluginButtonClicked()));
    QObject::connect(eventBridge, SIGNAL(addAssetDirectory(QString)),
                     this,        SLOT(onAddAssetDirectory(QString)));
    QObject::connect(eventBridge, SIGNAL(renameAssetDirectory(int,QString)),
                     this,        SLOT(onRenameAssetDirectory(int,QString)));
    QObject::connect(eventBridge, SIGNAL(removeAssetDirectory(int)),
                     this,        SLOT(onRemoveAssetDirectory(int)));
    QObject::connect(eventBridge, SIGNAL(openASIODriverControlPanel()),
                     this,        SLOT(onOpenASIODriverControlPanel()));
    QObject::connect(eventBridge, SIGNAL(exitASIOThread()),
                     this,        SLOT(onExitASIOThread()));
    // C++ -> C++
    QObject::connect(this,             &EventHandler::updatePluginList,
                     mainWindowEvents, &MainWindow::updatePluginList);
    QObject::connect(this,             &EventHandler::signalScanPluginComplete,
                     mainWindowEvents, &MainWindow::updatePluginList);
    QObject::connect(this,             &EventHandler::updateASIODriverList,
                     mainWindowEvents, &MainWindow::updateASIODriverList);
    // C++ -> QML
    QObject::connect(this,          SIGNAL(signalScanPluginComplete()),
                     optionsWindow, SIGNAL(scanPluginComplete()));
    QObject::connect(this,          SIGNAL(exitASIOThreadFinished()),
                     mainWindow,    SIGNAL(exitASIOThreadFinished()));
}

EventHandler::~EventHandler()
{
    //
}

void EventHandler::scanPluginComplete()
{
    using namespace Musec::UI;
    using namespace Musec::Event;
    mainWindow->setProperty("pluginList",
        QVariant::fromValue<QObject*>(&Musec::Controller::AppPluginList())
    );

    mainWindow->setProperty(
        "midiEffectList",
        QVariant::fromValue<QObject*>(&Musec::Controller::AppMidiEffectList())
    );
    mainWindow->setProperty(
        "instrumentList",
        QVariant::fromValue<QObject*>(&Musec::Controller::AppInstrumentList())
    );
    mainWindow->setProperty(
        "audioEffectList",
        QVariant::fromValue<QObject*>(&Musec::Controller::AppAudioEffectList())
    );
    eventHandler->updatePluginList();
    signalScanPluginComplete();
}


void EventHandler::onMainWindowOpened()
{
    using namespace Musec::Controller;
    loadASIODriver();
}

void EventHandler::onOptionsWindowOpened()
{
    using namespace Musec::Audio::Driver;
    using namespace Musec::UI;
    using namespace Musec::Event;
    // 加载插件目录
    auto pluginDirectoryList = Musec::Controller::getPluginDirectoryList();
    mainWindow->setProperty(
        "pluginDirectoryList",
        QVariant::fromValue<QStringList>(pluginDirectoryList)
    );
    // 加载 ASIO 驱动列表
    std::remove_reference_t<decltype(Musec::Controller::AppASIODriverList())> list;
    mainWindow->setProperty("driverList", QVariant::fromValue<QObject*>(&list));
    auto& driverList = Musec::Controller::AppASIODriverList();
    mainWindow->setProperty("driverList",
        QVariant::fromValue<QObject*>(&driverList));
    // 查找当前使用的 ASIO 驱动
    QString driverCLSID = Musec::Controller::getASIODriver();
    auto driverListCount = driverList.itemCount();
    auto driverListBase = driverList.getList();
    int driverCurrentIndex = 0;
    for(int i = 0; i < driverListCount; ++i)
    {
        using namespace Musec::Audio::Driver;
        if(std::get<ASIODriverField::CLSIDField>(driverListBase[i]) == driverCLSID)
        {
            driverCurrentIndex = i;
            break;
        }
    }
    mainWindow->setProperty("currentDriver",
        QVariant::fromValue<int>(driverCurrentIndex)
    );
    // 将选项窗口和 EventHandler 连接起来
    if(!optionsWindowConnection)
    {
        optionsWindowConnection =
            QObject::connect(eventBridge, SIGNAL(driverASIOSelectionChanged(QString)),
                             this,        SLOT(onDriverASIOSelectionChanged(QString)));
    }
}

void EventHandler::onOptionsWindowClosed()
{
    if(optionsWindowConnection)
    {
        QObject::disconnect(optionsWindowConnection);
        optionsWindowConnection = QMetaObject::Connection();
    }
}

void EventHandler::onPluginDirectoryAdded(const QString& directory)
{
    Musec::Controller::addPluginDirectory(directory);
}

void EventHandler::onScanPluginButtonClicked()
{
    using namespace Musec::UI;
    using namespace Musec::Event;
    constexpr char typeListNames[][16] = {"midiEffectList", "instrumentList", "audioEffectList"};
    auto empty = Musec::Model::PluginListModel();
    mainWindow->setProperty(
        "pluginList", QVariant::fromValue<QObject*>(&empty)
    );
    for(int i = 0; i < 3; ++i)
    {
        mainWindow->setProperty(
            typeListNames[i],
            QVariant::fromValue<QObject*>(&empty)
        );
    }
    Musec::Controller::refreshPluginList(true);
}

void EventHandler::onPluginDirectoryRemoved(const QString& directory)
{
    Musec::Controller::removePluginDirectory(directory);
}

void EventHandler::onAddAssetDirectory(const QString& directory)
{
    Musec::Controller::addAssetDirectory(directory);
    Musec::Event::mainWindowEvents->updateAssetDirectoryList();
}

void EventHandler::onRenameAssetDirectory(int id, const QString& name)
{
    Musec::Controller::renameAssetDirectory(id, name);
    Musec::Event::mainWindowEvents->updateAssetDirectoryList();
}

void EventHandler::onRemoveAssetDirectory(int id)
{
    Musec::Controller::removeAssetDirectory(id);
    Musec::Event::mainWindowEvents->updateAssetDirectoryList();
}

void EventHandler::onOpenASIODriverControlPanel()
{
    Musec::Controller::openASIODriverControlPanel();
}

void EventHandler::onDriverASIOSelectionChanged(const QString& clsid)
{
    if(clsid.length())
    {
        Musec::Controller::setASIODriver(clsid);
        Musec::Controller::loadASIODriver();
        Musec::Controller::updateCurrentASIODriverInfo();
    }
}

void EventHandler::onExitASIOThread()
{
    Musec::Controller::unloadASIODriver();
    exitASIOThreadFinished();
}
}
