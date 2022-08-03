#ifndef MUSEC_MODEL_PLUGINSEQUENCEMODEL
#define MUSEC_MODEL_PLUGINSEQUENCEMODEL

#include "audio/track/AudioTrack.hpp"
#include "audio/track/InstrumentTrack.hpp"
#include "audio/track/ITrack.hpp"
#include "entities/Plugin.hpp"
#include "model/ModelBase.hpp"

#include <QAbstractListModel>
#include <QMetaObject>

#include <memory>
#include <vector>

namespace Musec
{
namespace Model
{
class TrackListModel;

class PluginSequenceModel: public QAbstractListModel
{
    friend class TrackListModel;
    Q_OBJECT
public:
    using Item = Musec::Entities::Plugin;
    enum RoleNames
    {
        ValidRole = Qt::UserRole,
        ProcessingRole,
        NameRole,
        SidechainExistRole,
        SidechainEnabledRole,
        WindowVisibleRole,
        RoleNamesEnd
    };
public:
    explicit PluginSequenceModel(int trackIndex, QObject* parent = nullptr);
    explicit PluginSequenceModel();
    virtual ~PluginSequenceModel();
public:
    void initRoleNames();
    Q_INVOKABLE int itemCount() const;
    static constexpr int columnSize();
public:
    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Q_INVOKABLE
    Q_INVOKABLE
    void insert(std::shared_ptr<Musec::Audio::Plugin::IPlugin<float>> plugin, int index);
    void replace(std::shared_ptr<Musec::Audio::Plugin::IPlugin<float>> plugin, int index);
    void remove(int index);
    void clear();
protected:
    RoleNamesType roleNames() const override;
private:
    Musec::Audio::Track::InstrumentTrack* instrumentTrack_ = nullptr;
    Musec::Audio::Track::AudioTrack* audioTrack_ = nullptr;
    std::vector<std::unique_ptr<Musec::Entities::Plugin>> pluginWindowConnections_;
    std::vector<QMetaObject::Connection> connections_;
    RoleNamesType roleNames_;
};
}
}

#endif //MUSEC_MODEL_PLUGINSEQUENCEMODEL
