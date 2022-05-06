#ifndef MUSEC_MODEL_TRACKLISTMODEL
#define MUSEC_MODEL_TRACKLISTMODEL

#include "audio/engine/Project.hpp"
#include "model/ModelBase.hpp"
#include "model/PluginSequenceModel.hpp"

#include <QAbstractListModel>

namespace Musec
{
namespace Model
{
class TrackListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    enum RoleNames
    {
        NameRole = Qt::UserRole,
        TypeRole,
        ColorRole,
        HeightRole,
        MuteRole,
        SoloRole,
        InvertPhaseRole,
        ArmRecordingRole,
        InstrumentRole,
        PluginListRole,
        kRoleNamesCount
    };
public:
    explicit TrackListModel(QObject* parent = nullptr);
    virtual ~TrackListModel();
public:
    static constexpr int columnSize();
public:
    Q_INVOKABLE int trackCount() const noexcept;
public:
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex&, int) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
public:
    Q_INVOKABLE void insertTrack(int index, const Musec::Entities::CompleteTrack& track);
    Q_INVOKABLE void appendTrack(const Musec::Entities::CompleteTrack& track);
    Q_INVOKABLE void removeTrack(int index);
    Q_INVOKABLE void loadInstrument(int trackIndex, int pluginFormat, const QString& path, int pluginSubId);
    Q_INVOKABLE void insertEffect(int trackIndex, int pluginFormat, const QString& path, int pluginSubId, int pluginIndex);
    Q_INVOKABLE void replaceEffect(int trackIndex, int pluginFormat, const QString& path, int pluginSubId, int pluginIndex);
protected:
    virtual RoleNamesType roleNames() const override;
private:
    Musec::Audio::Engine::Project& project_;
    std::vector<std::unique_ptr<Musec::Model::PluginSequenceModel>> pluginSequences_;
    RoleNamesType roleNames_;
};
}
}

#endif //MUSEC_MODEL_TRACKLISTMODEL
