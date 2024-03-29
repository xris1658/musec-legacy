#ifndef MUSEC_UI_SPLASHSCREEN
#define MUSEC_UI_SPLASHSCREEN

#include "SplashScreenWorkerThread.hpp"

#include <QObject>

namespace Musec
{
namespace Event
{
class SplashScreen: public QObject
{
    Q_OBJECT
public:
    SplashScreen(QObject* parent = nullptr);
    ~SplashScreen() = default;
public slots:
    void onInitDialog();
signals:
    void setBootText(const QString& bootText);
    void openMainWindow();
    void closeDialog();
private:
    Musec::Event::SplashScreenWorkerThread workerThread_;
};
}
}

#endif // MUSEC_UI_SPLASHSCREEN
