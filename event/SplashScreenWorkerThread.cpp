#include "SplashScreenWorkerThread.hpp"
#include "controller/AppController.hpp"
// 此处需要 SplashScreen 的细节
#include "SplashScreen.hpp"

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
