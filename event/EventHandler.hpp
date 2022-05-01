#ifndef MUSEC_EVENT_EVENTHANDLER
#define MUSEC_EVENT_EVENTHANDLER

#include "entities/CompleteTrack.hpp"

#include <QObject>
#include <QQuickItem>

#include <vector>

namespace Musec
{
namespace Event
{
class EventHandler: public QObject
{
    Q_OBJECT
private:
    EventHandler(QObject* eventBridge, QObject* parent = nullptr);
public:
    virtual ~EventHandler();
    static EventHandler& instance(QObject* eventBridge);
public:
    void scanPluginComplete();
signals:
    void setStatusText(const QString& newText);
    void signalScanPluginComplete();
    void setSystemTextRenderingComplete();
    void updatePluginList();
    void updateASIODriverList();
    void exitASIOThreadFinished();
    void updateArrangementPosition(int position);
    void messageDialog(const QString& message, const QString& title, int icon);
    void requestExplorerViewComplete();
    void updateArrangement();
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
    void onSampleRateChanged(int sampleRate);
    void onSystemTextRenderingChanged(bool newValue);
    void onOpenSpecialCharacterInput();
    void onSetArrangementPosition(int position);
    void onPlayStart();
    void onPlayStop();
    void onRequestExplorerView();
    void onAppendTrack(Musec::Entities::CompleteTrack* track);
    void onInsertTrack(Musec::Entities::CompleteTrack* track, int index);
    void onTrackInserted(const QModelIndex& parent, int first, int last);
    void onTrackAboutToBeRemoved(const QModelIndex &parent, int first, int last);
private:
    std::vector<QMetaObject::Connection> optionsWindowConnection;
};
}
}

#endif // MUSEC_EVENT_EVENTHANDLER
