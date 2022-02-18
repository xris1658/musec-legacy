#ifndef MUSEC_EVENT_EVENTHANDLER
#define MUSEC_EVENT_EVENTHANDLER

#include <QObject>

#include <vector>

namespace Musec
{
namespace Event
{
class EventHandler: public QObject
{
    Q_OBJECT
public:
    EventHandler(QObject* eventBridge, QObject* parent = nullptr);
    virtual ~EventHandler();
public:
    void scanPluginComplete();
signals:
    void signalScanPluginComplete();
    void updatePluginList();
    void updateASIODriverList();
    void exitASIOThreadFinished();
public slots:
    void onMainWindowOpened();
    void onOptionsWindowOpened();
    void onOptionsWindowClosed();
    void onPluginDirectoryAdded(const QString& directory);
    void onPluginDirectoryRemoved(const QString& directory);
    void onScanPluginButtonClicked();
    void onAddAssetDirectory(const QString& directory);
    void onRenameAssetDirectory(int id, const QString& name);
    void onRemoveAssetDirectory(int id);
    void onOpenASIODriverControlPanel();
    void onDriverASIOSelectionChanged(const QString& clsid);
    void onExitASIOThread();
private:
    QMetaObject::Connection optionsWindowConnection;
};
}
}

#endif // MUSEC_EVENT_EVENTHANDLER
