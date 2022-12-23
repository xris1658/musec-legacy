#ifndef MUSEC_MODEL_PLUGINPARAMETERLISTMODEL
#define MUSEC_MODEL_PLUGINPARAMETERLISTMODEL

#include "audio/plugin/IPlugin.hpp"
#include "model/ModelBase.hpp"

#include <QAbstractListModel>

#include <memory>

namespace Musec
{
namespace Model
{
class PluginParameterListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    enum RoleNames
    {
        IdRole = Qt::UserRole,
        NameRole,
        ShortNameRole,
        DiscreteRole,
        PeriodicRole,
        HiddenRole,
        ReadonlyRole,
        AutomatableRole,
        MinValueRole,
        MaxValueRole,
        DefaultValueRole,
        ValueRole,
        StepRole,
        RoleNamesCount
    };
public:
    PluginParameterListModel(Musec::Audio::Plugin::IPlugin* plugin = nullptr, QObject* parent = nullptr);
    PluginParameterListModel(const PluginParameterListModel& rhs);
    ~PluginParameterListModel();
public:
    Q_INVOKABLE int parameterCount() const;
    int visibleParameterCount() const;
    int automatableParameterCount() const;
    static constexpr int columnSize();
public:
    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
protected:
    RoleNamesType roleNames() const override;
private:
    Musec::Audio::Plugin::IPlugin* plugin_;
    RoleNamesType roleNames_;
};

}
}

Q_DECLARE_METATYPE(Musec::Model::PluginParameterListModel);

#endif //MUSEC_MODEL_PLUGINPARAMETERLISTMODEL
