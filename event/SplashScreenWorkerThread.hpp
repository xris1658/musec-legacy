#ifndef MUSEC_UI_SPLASHSCREENWORKERTHREAD
#define MUSEC_UI_SPLASHSCREENWORKERTHREAD

#include "SplashScreenForward.hpp"

#include <QObject>
#include <QThread>

namespace Musec
{
namespace Event
{
class SplashScreenWorkerThread: public QThread
{
    Q_OBJECT
public:
    SplashScreenWorkerThread(SplashScreen* splashScreen, QObject* parent = nullptr);
    ~SplashScreenWorkerThread() = default;
protected:
    void run() override;
private:
    SplashScreen* splashScreen_;
};
}
}

#endif // MUSEC_UI_SPLASHSCREENWORKERTHREAD
