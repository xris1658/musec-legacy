#include "EventHandler.hpp"

#include "audio/driver/ASIODriver.hpp"
#include "controller/ASIODriverController.hpp"
#include "controller/AppController.hpp"
#include "controller/AssetController.hpp"
#include "controller/AssetDirectoryController.hpp"
#include "controller/AudioEngineController.hpp"
#include "controller/GeneralSettingsController.hpp"
#include "controller/MIDIClockController.hpp"
#include "controller/PluginSettingsController.hpp"
#include "event/EventBase.hpp"
#include "ui/PluginWindow.hpp"
#include "ui/UI.hpp"

#include <QDebug>

#include <chrono>

namespace Musec::Event
{
EventHandler::EventHandler(QObject* eventBridge, QObject* parent): QObject(parent)
{
    using namespace Musec::Event;
    using namespace Musec::UI;
    using Musec::Event::MainWindowEvent;
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
    QObject::connect(eventBridge, SIGNAL(prepareToQuit()),
                     this,        SLOT(onPrepareToQuit()));
    QObject::connect(eventBridge, SIGNAL(openSpecialCharacterInput()),
                     this,        SLOT(onOpenSpecialCharacterInput()));
    QObject::connect(eventBridge, SIGNAL(setArrangementPosition(int)),
                     this,        SLOT(onSetArrangementPosition(int)));
    QObject::connect(eventBridge, SIGNAL(playStart()),
                     this,        SLOT(onPlayStart()));
    QObject::connect(eventBridge, SIGNAL(playStop()),
                     this,        SLOT(onPlayStop()));
    QObject::connect(eventBridge, SIGNAL(requestExplorerView()),
                     this,        SLOT(onRequestExplorerView()));
    QObject::connect(eventBridge, SIGNAL(appendTrack(Musec::Entities::CompleteTrack*)),
                     this,        SLOT(onAppendTrack(Musec::Entities::CompleteTrack*)));
    QObject::connect(eventBridge, SIGNAL(insertTrack(Musec::Entities::CompleteTrack*, int)),
                     this,        SLOT(onInsertTrack(Musec::Entities::CompleteTrack*, int)));
    QObject::connect(eventBridge, SIGNAL(newPluginWindowReady()),
                     this,        SLOT(onNewPluginWindowReady()));
    QObject::connect(eventBridge, SIGNAL(setIcon()),
                     this,        SLOT(onSetIcon()));
    // (this) C++ -> C++ (other)
    QObject::connect(this,             &EventHandler::updatePluginList,
                     mainWindowEvents, &MainWindowEvent::updatePluginList);
    QObject::connect(this,             &EventHandler::signalScanPluginComplete,
                     mainWindowEvents, &MainWindowEvent::updatePluginList);
    QObject::connect(this,             &EventHandler::updateASIODriverList,
                     mainWindowEvents, &MainWindowEvent::updateASIODriverList);
    // (other) C++ -> C++ (this)
    QObject::connect(&(Musec::Controller::AudioEngineController::AppTrackListModel()),
                     &Musec::Model::TrackListModel::rowsInserted,
                     this,
                     &EventHandler::onTrackInserted);
    QObject::connect(&(Musec::Controller::AudioEngineController::AppTrackListModel()),
                     &Musec::Model::TrackListModel::rowsAboutToBeRemoved,
                     this,
                     &EventHandler::onTrackAboutToBeRemoved);
    // C++ -> QML
    QObject::connect(this,          SIGNAL(signalScanPluginComplete()),
                     optionsWindow, SIGNAL(scanPluginComplete()));
    QObject::connect(this,          SIGNAL(readyToQuit()),
                     mainWindow,    SIGNAL(readyToQuit()));
    QObject::connect(this,          SIGNAL(setStatusText(QString)),
                     mainWindow,    SIGNAL(setStatusText(QString)));
    QObject::connect(this,          SIGNAL(setSystemTextRenderingComplete()),
                     eventBridge,   SIGNAL(setSystemTextRenderingComplete()));
    QObject::connect(this,          SIGNAL(updateArrangementPosition(int)),
                     eventBridge,   SIGNAL(updateArrangementPosition(int)));
    QObject::connect(this,          SIGNAL(messageDialog(QString, QString, int)),
                     eventBridge,   SIGNAL(messageDialog(QString, QString, int)));
    QObject::connect(this,          SIGNAL(requestExplorerViewComplete()),
                     eventBridge,   SIGNAL(requestExplorerViewComplete()));
    QObject::connect(this,          SIGNAL(updateArrangement()),
                     eventBridge,   SIGNAL(updateArrangement()));
    QObject::connect(this,          SIGNAL(newPluginWindow()),
                     eventBridge,   SIGNAL(newPluginWindow()));
    QObject::connect(this,          SIGNAL(updateUsage(double)),
                     eventBridge,   SIGNAL(updateUsage(double)));
}

EventHandler::~EventHandler()
{
    //
}

EventHandler& EventHandler::instance(QObject* eventBridge)
{
    static EventHandler ret(eventBridge);
    return ret;
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
    updatePluginList();
    signalScanPluginComplete();
}


void EventHandler::onMainWindowOpened()
{
    Controller::ASIODriverController::loadASIODriver();
}

void EventHandler::onOptionsWindowOpened()
{
    using namespace Musec::Audio::Driver;
    using namespace Musec::UI;
    using namespace Musec::Event;
    // 常规设置
    auto systemRender = Musec::Controller::ConfigController::appConfig()["musec"]["options"]["general"]["system-render"].as<bool>();
    optionsWindow->setProperty(
        "systemRender",
        QVariant::fromValue(systemRender)
    );
    // 加载插件目录
    auto pluginDirectoryList = Musec::Controller::PluginSettingsController::getPluginDirectoryList();
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
    QString driverCLSID = Controller::ASIODriverController::getASIODriver();
    auto driverListCount = driverList.itemCount();
    auto driverListBase = driverList.getList();
    int driverCurrentIndex = -1;
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
    // 将选项窗口的事件和 EventHandler 连接起来
    if(optionsWindowConnection.empty())
    {
        optionsWindowConnection.emplace_back(
            QObject::connect(
                eventBridge, SIGNAL(driverASIOSelectionChanged(QString)),
                this,        SLOT(onDriverASIOSelectionChanged(QString))
            )
        );
        optionsWindowConnection.emplace_back(
            QObject::connect(
                eventBridge, SIGNAL(sampleRateChanged(int)),
                this,        SLOT(onSampleRateChanged(int))
            )
        );
        optionsWindowConnection.emplace_back(
            QObject::connect(
                eventBridge, SIGNAL(systemTextRenderingChanged(bool)),
                this,        SLOT(onSystemTextRenderingChanged(bool))
            )
        );
        optionsWindowConnection.emplace_back(
            QObject::connect(
                eventBridge, SIGNAL(scanShortcutChanged(bool)),
                this,        SLOT(onScanShortcutChanged(bool))
            )
        );
    }
}

void EventHandler::onOptionsWindowClosed()
{
    for(auto& connection: optionsWindowConnection)
    {
        QObject::disconnect(connection);
    }
    optionsWindowConnection.clear();
}

void EventHandler::onPluginDirectoryAdded(const QString& directory)
{
    Musec::Controller::PluginSettingsController::addPluginDirectory(directory);
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
    Musec::Controller::PluginSettingsController::removePluginDirectory(directory);
}

void EventHandler::onAddAssetDirectory(const QString& directory)
{
    Controller::AssetDirectoryController::addAssetDirectory(directory);
}

void EventHandler::onRenameAssetDirectory(int id, const QString& name)
{
    Controller::AssetDirectoryController::renameAssetDirectory(id, name);
}

void EventHandler::onRemoveAssetDirectory(int id)
{
    Controller::AssetDirectoryController::removeAssetDirectory(id);
}

void EventHandler::onOpenASIODriverControlPanel()
{
    Controller::ASIODriverController::openASIODriverControlPanel();
}

void EventHandler::onDriverASIOSelectionChanged(const QString& clsid)
{
    if(clsid.length())
    {
        Controller::ASIODriverController::setASIODriver(clsid);
    }
}

void EventHandler::onPrepareToQuit()
{
    Controller::ASIODriverController::unloadASIODriver();
    Musec::Controller::AudioEngineController::AppProject().clear();
    readyToQuit();
}

void EventHandler::onSampleRateChanged(int sampleRate)
{
    using namespace Musec::Audio::Driver;
    // 使用 decltype 的推导规则进行自动推导。多在编写程序库时使用。
    // 和单例模式一并使用效果绝佳。
    // （话说回来这儿直接用 auto& 也没问题。）
    decltype(auto) driver = AppASIODriver();
    if(driver)
    {
        driver->setSampleRate(static_cast<ASIOSampleRate>(sampleRate));
    }
}

void EventHandler::onSystemTextRenderingChanged(bool newValue)
{
    Musec::Controller::GeneralSettingsController::setSystemTextRendering(newValue);
    setSystemTextRenderingComplete();
}

void EventHandler::onOpenSpecialCharacterInput()
{
    Musec::Controller::openSpecialCharacterInput();
}

void EventHandler::onSetArrangementPosition(int position)
{
    auto timePoint = Musec::Audio::Base::TimePoint<Musec::Controller::MIDIClockController::AppPPQ>(position);
    Musec::Controller::MIDIClockController::AppMIDIClock().setPosition(timePoint);
    Musec::Controller::MIDIClockController::updateArrangementPositionInUI(timePoint);
}

void EventHandler::onPlayStart()
{
    Musec::Controller::MIDIClockController::AppMIDIClock().play();
}

void EventHandler::onPlayStop()
{
    Musec::Controller::MIDIClockController::AppMIDIClock().stop();
}

void EventHandler::onRequestExplorerView()
{
    using namespace Musec::UI;
    auto explorerView = mainWindow->property("explorerViewOnRequest").value<QObject*>();
    if(!explorerView)
    {
        return;
    }
    auto newFolderListModel = new Musec::Model::FolderListModel(explorerView);
    auto newFileListModel = new Musec::Model::FileListModel(explorerView);
    auto setList = [this, &explorerView, &newFileListModel, &newFolderListModel]()
    {
        using namespace Musec::UI;
        auto path = explorerView->property("path").value<QString>();
        auto folderList = Musec::Controller::AssetController::getFolderInDirectory(path);
        auto fileList = Musec::Controller::AssetController::getFileInDirectory(path);
        newFolderListModel->setList(folderList);
        newFileListModel->setList(fileList);
        explorerView->setProperty("expandableItemList", QVariant::fromValue(newFolderListModel));
        explorerView->setProperty("nonExpandableItemList", QVariant::fromValue(newFileListModel));
        requestExplorerViewComplete();
    };
    std::async(std::launch::async, setList);
}

void EventHandler::onAppendTrack(Musec::Entities::CompleteTrack* track)
{
    Musec::Controller::AudioEngineController::appendTrack(*track);
}

void EventHandler::onInsertTrack(Musec::Entities::CompleteTrack* track, int index)
{
    Musec::Controller::AudioEngineController::insertTrack(index, *track);
}

void EventHandler::onTrackInserted(const QModelIndex& parent, int first, int last)
{
}

void EventHandler::onTrackAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    emit updateArrangement();
}

void EventHandler::onNewPluginWindowReady()
{
    auto window = Musec::UI::mainWindow->property("windowForPlugin").value<QWindow*>();
    Musec::UI::createNewPluginWindowComplete(window);
}

void EventHandler::onSetIcon()
{
    auto window = Musec::UI::mainWindow->property("windowToSetIcon").value<QWindow*>();
    Musec::UI::setIcon(window);
}

void EventHandler::onScanShortcutChanged(bool newValue)
{
    Musec::Controller::PluginSettingsController::setScanShortcut(newValue);
}
}
