#ifndef MUSEC_UI_MAINWINDOW
#define MUSEC_UI_MAINWINDOW

#include "event/SplashScreen.hpp"

#include <QObject>

namespace Musec
{
namespace Event
{
class MainWindow: public QObject
{
    Q_OBJECT
public:
    MainWindow(const Musec::Event::SplashScreen& splash);
public slots:
    void openMainWindow();
signals:
    void openMainWindowComplete();
public:
    void updateAssetDirectoryList();
    void updateASIODriverList();
    void updatePluginList();
};
}
}



#endif // MUSEC_UI_MAINWINDOW
