#ifndef MUSEC_UI_SPLASHSCREENWORKERTHREAD
#define MUSEC_UI_SPLASHSCREENWORKERTHREAD

// 类声明中需要 SplashScreen 的类型声明，因此在此处添加前向声明。
// 或许把这些东西放到一个类中更好，但是 Qt 的元对象系统不支持嵌套类。
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
