#include "MainWindowEvent.hpp"

#include "audio/driver/ASIODriver.hpp"
#include "concurrent/ThreadId.hpp"
#include "controller/ASIODriverController.hpp"
#include "controller/AppController.hpp"
#include "controller/AudioEngineController.hpp"
#include "controller/MIDIClockController.hpp"
#include "event/EventBase.hpp"
#include "ui/UI.hpp"

#include <QString>
#include <QQuickWindow>

namespace Musec::Event
{
MainWindowEvent::MainWindowEvent(const Musec::Event::SplashScreen& splash): QObject(nullptr)
{
    QObject::connect(&splash, &Musec::Event::SplashScreen::openMainWindow,
                     this,    &MainWindowEvent::openMainWindow);
    QObject::connect(this,    &MainWindowEvent::openMainWindowComplete,
                     &splash, &Musec::Event::SplashScreen::closeDialog);
}

void MainWindowEvent::openMainWindow()
{
    using namespace Musec::UI;
    using namespace Musec::Event;
    assert(std::this_thread::get_id() == Musec::Concurrent::mainThreadId());
    Musec::Controller::AudioEngineController::initializeFacility();
    engine->load(QUrl("qrc:///qmlproject/Musec.qml"));
    mainWindow = qobject_cast<QQuickWindow*>(engine->rootObjects()[1]);
    mainWindow->setIcon(QIcon(":/qmlproject/images/Musec-image-2.ico"));
    optionsWindow = mainWindow->findChild<QQuickWindow*>("optionsWindow");

    Musec::Event::eventHandler->connectToMainWindow();
    Musec::Event::eventHandler->connectToOptionsWindow();
    // auto refreshRate = Musec::UI::refreshRateInHertz();

    // I originally planned to set the interval of the realtime timer according to the refresh rate,
    // only to realize that QML works suboptimal on integrated graphics device.
    // There might be a better solution than this.
    auto interval = 100;
    eventHandler->setRealtimeTimerInterval(interval);
    QObject::connect(eventHandler, &Musec::Event::EventHandler::updatePluginList,
                     this, &Musec::Event::MainWindowEvent::updatePluginList);
    QObject::connect(this, &Musec::Event::MainWindowEvent::openMainWindowComplete,
                     eventHandler, &Musec::Event::EventHandler::onMainWindowOpened);
    // Musec.qml, Musec.onActiveFocusItemChanged()
    auto visibility = mainWindow->visibility();
    mainWindow->showFullScreen();
    mainWindow->setVisibility(visibility);
    updateAssetDirectoryList();
    updatePluginList();
    mainWindow->setProperty("trackList", QVariant::fromValue(&Musec::Controller::AudioEngineController::AppTrackListModel()));
    mainWindow->setProperty("masterTrackPluginSequence", QVariant::fromValue(&Musec::Controller::AudioEngineController::AppTrackListModel().masterTrackPluginSequenceModel()));
    auto& midiClock = Musec::Controller::MIDIClockController::AppMIDIClock();
    midiClock.tempoAutomation().insertPoint({0U, 128.0, 1.0});
    mainWindow->setProperty("bpm", QVariant::fromValue(Musec::Controller::AudioEngineController::getCurrentTempo())); // FIXME
    openMainWindowComplete();
}

void MainWindowEvent::updateAssetDirectoryList()
{
    using namespace Musec::UI;
    assert(std::this_thread::get_id() == Musec::Concurrent::mainThreadId());
    std::remove_reference_t<decltype(Musec::Controller::AppAssetDirectoryList())> list;
    mainWindow->setProperty("assetDirectoryList", QVariant::fromValue<QObject*>(&list));
    mainWindow->setProperty(
        "assetDirectoryList",
        QVariant::fromValue<QObject*>(&Musec::Controller::AppAssetDirectoryList()));
}

void MainWindowEvent::updateASIODriverList()
{
    using namespace Musec::UI;
    assert(std::this_thread::get_id() == Musec::Concurrent::mainThreadId());
    std::remove_reference_t<decltype(Musec::Controller::AppASIODriverList())> list;
    mainWindow->setProperty("driverList", QVariant::fromValue<QObject*>(&list));
    auto& driverList = Musec::Controller::AppASIODriverList();
    mainWindow->setProperty("driverList",
        QVariant::fromValue<QObject*>(&driverList));
    QString driverCLSID = Controller::ASIODriverController::getASIODriver();
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
}

void MainWindowEvent::updatePluginList()
{
    using namespace Musec::UI;
    assert(std::this_thread::get_id() == Musec::Concurrent::mainThreadId());
    mainWindow->setProperty(
        "pluginList",
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
}
}
