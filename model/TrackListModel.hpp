#ifndef MUSEC_MODEL_TRACKLISTMODEL
#define MUSEC_MODEL_TRACKLISTMODEL

#include "audio/engine/Project.hpp"
#include "model/ModelBase.hpp"

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
    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex&, int) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
public:
    Q_INVOKABLE void insertTrack(int index, const Musec::Entities::CompleteTrack& track);
    Q_INVOKABLE void appendTrack(const Musec::Entities::CompleteTrack& track);
    Q_INVOKABLE void removeTrack(int index);

protected:
    virtual RoleNamesType roleNames() const override;
private:
    Musec::Audio::Engine::Project& project_;
    RoleNamesType roleNames_;
};
}
}

#endif //MUSEC_MODEL_TRACKLISTMODEL
