#include "PluginParameterListModel.hpp"

#include "audio/plugin/IParameter.hpp"

namespace Musec::Model
{
PluginParameterListModel::PluginParameterListModel(Musec::Audio::Plugin::IPlugin* plugin, QObject* parent):
    QAbstractListModel(parent),
    plugin_(plugin)
{
    roleNames_.reserve(columnSize());
    roleNames_[IdRole] = "parameterId";
    roleNames_[NameRole] = "name";
    roleNames_[ShortNameRole] = "shortName";
    roleNames_[DiscreteRole] = "discrete";
    roleNames_[PeriodicRole] = "periodic";
    roleNames_[HiddenRole] = "hidden";
    roleNames_[ReadonlyRole] = "readonly";
    roleNames_[AutomatableRole] = "automatable";
    roleNames_[MinValueRole] = "minValue";
    roleNames_[MaxValueRole] = "maxValue";
    roleNames_[DefaultValueRole] = "defaultValue";
    roleNames_[ValueRole] = "value";
    roleNames_[StepRole] = "step";
}

PluginParameterListModel::PluginParameterListModel(const PluginParameterListModel& rhs):
    QAbstractListModel(rhs.parent()),
    plugin_(rhs.plugin_),
    roleNames_(rhs.roleNames_)
{
    //
}

PluginParameterListModel::~PluginParameterListModel()
{
    //
}

Q_INVOKABLE int PluginParameterListModel::parameterCount() const
{
    return plugin_? plugin_->parameterCount(): 0;
}

int PluginParameterListModel::visibleParameterCount() const
{
    return 0;
}

int PluginParameterListModel::automatableParameterCount() const
{
    return 0;
}

constexpr int PluginParameterListModel::columnSize()
{
    return RoleNames::RoleNamesCount - Qt::UserRole;
}

int PluginParameterListModel::rowCount(const QModelIndex&) const
{
    return parameterCount();
}

int PluginParameterListModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant PluginParameterListModel::data(const QModelIndex& index, int role) const
{
    using namespace Musec::Audio::Plugin;
    int row = index.row();
    if(row >= 0 && row < parameterCount())
    {
        auto& parameter = plugin_->parameter(row);
        switch(role)
        {
        case IdRole:
            return QVariant::fromValue(row);
        case NameRole:
            return QVariant::fromValue(parameter.name());
        case ShortNameRole:
            return QVariant::fromValue(QString());
        case DiscreteRole:
            return QVariant::fromValue((parameter.flags() & ParameterFlags::Discrete) != 0);
        case PeriodicRole:
            return QVariant::fromValue((parameter.flags() & ParameterFlags::Periodic) != 0);
        case HiddenRole:
            return QVariant::fromValue((parameter.flags() & ParameterFlags::Hidden) != 0);
        case ReadonlyRole:
            return QVariant::fromValue((parameter.flags() & ParameterFlags::Readonly) != 0);
        case AutomatableRole:
            return QVariant::fromValue((parameter.flags() & ParameterFlags::Automatable) != 0);
        case MinValueRole:
            return QVariant::fromValue(parameter.minValue());
        case MaxValueRole:
            return QVariant::fromValue(parameter.maxValue());
        case ValueRole:
            return QVariant::fromValue(parameter.value());
        case StepRole:
            return QVariant::fromValue(parameter.step());
        default:
            return QVariant();
        }
    }
}

bool PluginParameterListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    return QAbstractItemModel::setData(index, value, role);
}

RoleNamesType PluginParameterListModel::roleNames() const
{
    return roleNames_;
}
}