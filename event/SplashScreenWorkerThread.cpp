#include "SplashScreenWorkerThread.hpp"
#include "controller/AppController.hpp"

namespace Musec
{
namespace Event
{
SplashScreenWorkerThread::SplashScreenWorkerThread(SplashScreen* splashScreen, QObject* parent):
    QThread(parent), splashScreen_(splashScreen)
{
    //
}

SplashScreenWorkerThread::~SplashScreenWorkerThread()
{
    //
}

void SplashScreenWorkerThread::run()
{
    Musec::Controller::initApplication(this->splashScreen_);
}
}
}
