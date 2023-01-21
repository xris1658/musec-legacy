#include "EventHandler.hpp"

#include "audio/driver/ASIOCallback.hpp"
#include "audio/driver/ASIODriver.hpp"
#include "concurrent/ThreadId.hpp"
#include "controller/ASIODriverController.hpp"
#include "controller/AppController.hpp"
#include "controller/AssetController.hpp"
#include "controller/AssetDirectoryController.hpp"
#include "controller/AudioEngineController.hpp"
#include "controller/ConfigController.hpp"
#include "controller/GeneralSettingsController.hpp"
#include "controller/MIDIClockController.hpp"
#include "controller/PluginController.hpp"
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
    QObject::connect(eventBridge, SIGNAL(newPluginWindowReady()),
                     this,        SLOT(onNewPluginWindowReady()));
    QObject::connect(eventBridge, SIGNAL(newBasicPluginEditorReady()),
                     this,        SLOT(onNewBasicPluginEditorReady()));
    QObject::connect(eventBridge, SIGNAL(setIcon()),
                     this,        SLOT(onSetIcon()));
    QObject::connect(eventBridge, SIGNAL(updateCPUMeter()),
                     this,        SLOT(onUpdateCPUMeter()));
    QObject::connect(eventBridge, SIGNAL(reloadDriver()),
                     this,        SLOT(onResetASIODriver()));
    // (this) C++ -> C++ (this)
    QObject::connect(this, &EventHandler::resetASIODriver,
                     this, &EventHandler::onResetASIODriver,
                     Qt::ConnectionType::QueuedConnection);
    QObject::connect(this, &EventHandler::backendScanPluginComplete,
                     this, &EventHandler::scanPluginComplete,
                     Qt::ConnectionType::QueuedConnection);
    QObject::connect(this, &EventHandler::backendRequestExplorerViewComplete,
                     this, &EventHandler::onBackendRequestExplorerViewComplete,
                     Qt::ConnectionType::QueuedConnection);
    // (this) C++ -> C++ (other)
    QObject::connect(this,             &EventHandler::updatePluginList,
                     mainWindowEvents, &MainWindowEvent::updatePluginList);
    QObject::connect(this,             &EventHandler::notifyScanPluginComplete,
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

    QObject::connect(this,          SIGNAL(setSystemTextRenderingComplete()),
                     eventBridge,   SIGNAL(setSystemTextRenderingComplete()));
    QObject::connect(this,          SIGNAL(updateArrangementPosition(int)),
                     eventBridge,   SIGNAL(updateArrangementPosition(int)));
    QObject::connect(this,          SIGNAL(messageDialog(QString, QString, int)),
                     eventBridge,   SIGNAL(messageDialog(QString, QString, int)));
    QObject::connect(this,          SIGNAL(messageDialogNonModal(QString, QString, int)),
                     eventBridge,   SIGNAL(messageDialogNonModal(QString, QString, int)));
    QObject::connect(this,          SIGNAL(requestExplorerViewComplete()),
                     eventBridge,   SIGNAL(requestExplorerViewComplete()));
    QObject::connect(this,          SIGNAL(updateArrangement()),
                     eventBridge,   SIGNAL(updateArrangement()));
    QObject::connect(this,          SIGNAL(newPluginWindow()),
                     eventBridge,   SIGNAL(newPluginWindow()));
    QObject::connect(this,          SIGNAL(newBasicPluginEditor()),
                     eventBridge,   SIGNAL(newBasicPluginEditor()));
    QObject::connect(this,          SIGNAL(updateUsage(double)),
                     eventBridge,   SIGNAL(updateUsage(double)));
    QObject::connect(this,          SIGNAL(setLanguageComplete()),
                     eventBridge,   SIGNAL(setLanguageComplete()));
    QObject::connect(this,          SIGNAL(setRealtimeTimerInterval(int)),
                     eventBridge,   SIGNAL(setRealtimeTimerInterval(int)));

    optionsWindowConnection_.reserve(5); // number of connections added in onOptionsWindowOpened()
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
    assert(std::this_thread::get_id() == Musec::Concurrent::mainThreadId());
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
    notifyScanPluginComplete();
}

void EventHandler::onMainWindowOpened()
{
    using namespace Musec::Controller::ASIODriverController;
    setASIODriver(getASIODriver());
}

void EventHandler::onOptionsWindowOpened()
{
    using namespace Musec::Audio::Driver;
    using namespace Musec::UI;
    using namespace Musec::Event;
    assert(std::this_thread::get_id() == Musec::Concurrent::mainThreadId());
    auto& translationList = Musec::Controller::AppTranslationFileList();
    auto currentLanguage = QString::fromStdString(
        Musec::Controller::ConfigController::appConfig()
        ["musec"]["options"]["general"]["language"]
            .as<std::string>()
    );
    auto systemRender = Musec::Controller::ConfigController::appConfig()
        ["musec"]["options"]["general"]["system-render"]
            .as<bool>();
    optionsWindow->setProperty(
        "languageList",
        QVariant::fromValue<QObject*>(&translationList)
    );
    auto itemCount = translationList.itemCount();
    for(decltype(itemCount) i = 0; i < itemCount; ++i)
    {
        using Musec::Model::TranslationFileModel;
        if(std::get<TranslationFileModel::RoleNames::LanguageNameRole - Qt::UserRole>(translationList[i]) == currentLanguage)
        {
            optionsWindow->setProperty(
                "currentLanguage",
                QVariant::fromValue(i)
            );
            break;
        }
    }
    optionsWindow->setProperty(
        "systemRender",
        QVariant::fromValue(systemRender)
    );
    auto pluginDirectoryList = Musec::Controller::PluginSettingsController::getPluginDirectoryList();
    mainWindow->setProperty(
        "pluginDirectoryList",
        QVariant::fromValue<QStringList>(pluginDirectoryList)
    );
    std::remove_reference_t<decltype(Musec::Controller::AppASIODriverList())> list;
    mainWindow->setProperty("driverList", QVariant::fromValue<QObject*>(&list));
    auto& driverList = Musec::Controller::AppASIODriverList();
    mainWindow->setProperty("driverList",
        QVariant::fromValue<QObject*>(&driverList));
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
    if(optionsWindowConnection_.empty())
    {
        optionsWindowConnection_.emplace_back(
            QObject::connect(
                eventBridge, SIGNAL(driverASIOSelectionChanged(QString)),
                this,        SLOT(onDriverASIOSelectionChanged(QString))
            )
        );
        optionsWindowConnection_.emplace_back(
            QObject::connect(
                eventBridge, SIGNAL(sampleRateChanged(int)),
                this,        SLOT(onSampleRateChanged(int))
            )
        );
        optionsWindowConnection_.emplace_back(
            QObject::connect(
                eventBridge, SIGNAL(systemTextRenderingChanged(bool)),
                this,        SLOT(onSystemTextRenderingChanged(bool))
            )
        );
        optionsWindowConnection_.emplace_back(
            QObject::connect(
                eventBridge, SIGNAL(scanShortcutChanged(bool)),
                this,        SLOT(onScanShortcutChanged(bool))
            )
        );
        optionsWindowConnection_.emplace_back(
            QObject::connect(
                eventBridge, SIGNAL(languageSelectionChanged(QString)),
                this,        SLOT(onLanguageSelectionChanged(QString))
            )
        );
    }
}

void EventHandler::onOptionsWindowClosed()
{
    for(auto& connection: optionsWindowConnection_)
    {
        QObject::disconnect(connection);
    }
    optionsWindowConnection_.clear();
}

void EventHandler::onPluginDirectoryAdded(const QString& directory)
{
    Musec::Controller::PluginSettingsController::addPluginDirectory(directory);
}

void EventHandler::onScanPluginButtonClicked()
{
    using namespace Musec::UI;
    using namespace Musec::Event;
    assert(std::this_thread::get_id() == Musec::Concurrent::mainThreadId());
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
        Musec::Controller::ASIODriverController::setASIODriver(clsid);
    }
}

void EventHandler::onPrepareToQuit()
{
    Musec::Controller::ASIODriverController::unloadASIODriver();
    Musec::Controller::AudioEngineController::AppTrackListModel().clear();
    Musec::Controller::AudioEngineController::AppProject().clear();
    readyToQuit();
}

void EventHandler::onSampleRateChanged(int sampleRate)
{
    using namespace Musec::Audio::Driver;
    auto& driver = AppASIODriver();
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
    explorerView_ = mainWindow->property("explorerViewOnRequest").value<QObject*>();
    if(!explorerView_)
    {
        return;
    }
    folderListModel_ = new Musec::Model::FolderListModel(explorerView_);
    fileListModel_ = new Musec::Model::FileListModel(explorerView_);
    auto setList = [this]()
    {
        using namespace Musec::UI;
        auto path = explorerView_->property("path").value<QString>();
        auto folderList = Musec::Controller::AssetController::getFolderInDirectory(path);
        auto fileList = Musec::Controller::AssetController::getFileInDirectory(path);
        folderListModel_->setList(folderList);
        fileListModel_->setList(fileList);
        eventHandler->backendRequestExplorerViewComplete();
    };
    std::async(std::launch::async, setList);
}

void EventHandler::onTrackInserted(const QModelIndex& parent, int first, int last)
{
}

void EventHandler::onTrackAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    updateArrangement();
}

void EventHandler::onNewPluginWindowReady()
{
    auto window = Musec::UI::mainWindow->property("windowForPlugin").value<QWindow*>();
    Musec::UI::createNewPluginWindowComplete(window);
}

void EventHandler::onNewBasicPluginEditorReady()
{
    auto window = Musec::UI::mainWindow->property("basicPluginEditorForPlugin").value<QWindow*>();
    Musec::UI::createBasicPluginEditorComplete(window);
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

void EventHandler::onLanguageSelectionChanged(const QString& language)
{
    Musec::Controller::GeneralSettingsController::setLanguage(language);
    setLanguageComplete();
}

void EventHandler::onUpdateCPUMeter()
{
    auto cpuUsage = Musec::Controller::AudioEngineController::getCpuUsage();
    updateUsage(cpuUsage);
}

void EventHandler::onResetASIODriver()
{
    Musec::Controller::ASIODriverController::resetASIODriver();
}

void EventHandler::onBackendRequestExplorerViewComplete()
{
    assert(std::this_thread::get_id() == Musec::Concurrent::mainThreadId());
    explorerView_->setProperty("expandableItemList", QVariant::fromValue(folderListModel_));
    explorerView_->setProperty("nonExpandableItemList", QVariant::fromValue(fileListModel_));
    requestExplorerViewComplete();
}

void EventHandler::connectToMainWindow()
{
    using namespace Musec::UI;
    QObject::connect(this,          SIGNAL(readyToQuit()),
                     mainWindow,    SIGNAL(readyToQuit()));
    QObject::connect(this,          SIGNAL(setStatusText(QString)),
                     mainWindow,    SIGNAL(setStatusText(QString)));
}

void EventHandler::connectToOptionsWindow()
{
    using namespace Musec::UI;
    QObject::connect(this,          SIGNAL(notifyScanPluginComplete()),
                     optionsWindow, SIGNAL(scanPluginComplete()));
}
}
