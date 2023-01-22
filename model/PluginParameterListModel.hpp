#ifndef MUSEC_MODEL_PLUGINPARAMETERLISTMODEL
#define MUSEC_MODEL_PLUGINPARAMETERLISTMODEL

#include "audio/plugin/IPlugin.hpp"
#include "model/ModelBase.hpp"
#include "model/PluginParameterValueListModel.hpp"

#include <QAbstractListModel>

#include <map>

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
        ShowAsListRole,
        ShowAsSwitchRole,
        ListRole,
        RoleEnd
    };
public:
    PluginParameterListModel(Musec::Audio::Plugin::IPlugin* plugin = nullptr, QObject* parent = nullptr);
    PluginParameterListModel(const PluginParameterListModel& rhs);
    ~PluginParameterListModel();
public:
    Q_INVOKABLE int parameterCount() const;
    Q_INVOKABLE QString valueToString(int index, double value) const;
    static constexpr int columnSize() noexcept;
public:
    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
protected:
    RoleNamesType roleNames() const override;
private:
    Musec::Audio::Plugin::IPlugin* plugin_;
    // data() might modify this by inserting elements, so use `mutable`
    mutable std::map<int, Musec::Model::PluginParameterValueListModel> valueList_;
};

}
}

Q_DECLARE_METATYPE(Musec::Model::PluginParameterListModel);

#endif //MUSEC_MODEL_PLUGINPARAMETERLISTMODEL
