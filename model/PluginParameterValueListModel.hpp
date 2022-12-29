#ifndef MUSEC_AUDIO_PLUGIN_PLUGINPARAMETERVALUELISTMODEL
#define MUSEC_AUDIO_PLUGIN_PLUGINPARAMETERVALUELISTMODEL

#include "audio/plugin/IParameter.hpp"
#include "model/ModelBase.hpp"

#include <QAbstractListModel>

namespace Musec
{
namespace Model
{
class PluginParameterValueListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    enum RoleNames
    {
        ValueRole = Qt::UserRole,
        TextRole,
        RoleEnd
    };
public:
    PluginParameterValueListModel(Musec::Audio::Plugin::IParameter* parameter = nullptr, QObject* parent = nullptr);
    PluginParameterValueListModel(const PluginParameterValueListModel& rhs);
    ~PluginParameterValueListModel();
public:
    Q_INVOKABLE int valueCount() const;
    static constexpr int columnSize();
public:
    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;
protected:
    RoleNamesType roleNames() const override;
public:
    Musec::Audio::Plugin::IParameter* parameter_;
private:
    RoleNamesType roleNames_;
};
}
}

Q_DECLARE_METATYPE(Musec::Model::PluginParameterValueListModel);

#endif //MUSEC_AUDIO_PLUGIN_PLUGINPARAMETERVALUELISTMODEL
