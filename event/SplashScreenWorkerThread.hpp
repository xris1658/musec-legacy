#ifndef MUSEC_UI_SPLASHSCREENWORKERTHREAD
#define MUSEC_UI_SPLASHSCREENWORKERTHREAD

// 类声明中需要 SplashScreen 的类型声明，因此在此处添加前向声明。
// 或许把这些东西放到一个类中更好，但是连接信号和槽函数时
// Qt 会闹脾气，只好作罢。日后再研究。
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
    virtual ~SplashScreenWorkerThread();
protected:
    void run() override;
private:
    SplashScreen* splashScreen_;
};
}
}

#endif // MUSEC_UI_SPLASHSCREENWORKERTHREAD
