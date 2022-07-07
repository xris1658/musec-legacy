#ifndef MUSEC_UI_MAINWINDOW
#define MUSEC_UI_MAINWINDOW

#include "event/SplashScreen.hpp"
#include "model/FileListModel.hpp"
#include "model/FolderListModel.hpp"

#include <QObject>

namespace Musec
{
namespace Event
{
class MainWindowEvent: public QObject
{
    Q_OBJECT
public:
    MainWindowEvent(const Musec::Event::SplashScreen& splash);
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
