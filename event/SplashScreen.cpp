#include "SplashScreen.hpp"

#include "controller/AppController.hpp"
#include "event/EventBase.hpp"
#include "ui/UI.hpp"

#include <QCoreApplication>

namespace Musec::Event
{
SplashScreen::SplashScreen(QObject* parent): QObject(parent), workerThread(this)
{
    using namespace Musec::UI;
    // C++ -> C++
    QObject::connect(&workerThread, &SplashScreenWorkerThread::finished,
                     this,          &SplashScreen::openMainWindow);
    // C++ -> QML
    QObject::connect(this,         SIGNAL(closeDialog()),
                     splashWindow, SIGNAL(closeDialog()));
    QObject::connect(this,         SIGNAL(setBootText(QString)),
                     splashWindow, SIGNAL(setBootText(QString)));
}

SplashScreen::~SplashScreen()
{
    //
}

void SplashScreen::onInitDialog()
{
    workerThread.start();
}
}
