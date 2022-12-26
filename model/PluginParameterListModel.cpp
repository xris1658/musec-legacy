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
    roleNames_[ShowAsListRole] = "showAsList";
    roleNames_[ShowAsSwitchRole] = "showAsSwitch";
    roleNames_[ListRole] = "list";
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

Q_INVOKABLE QString PluginParameterListModel::valueToString(int index, double value) const
{
    return plugin_->parameter(index).valueToString(value);
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
            return QVariant::fromValue(parameter.id());
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
            return QVariant::fromValue(parameter.stepSize());
        case ShowAsListRole:
            return QVariant::fromValue((parameter.flags() & ParameterFlags::ShowAsList) != 0);
        case ShowAsSwitchRole:
            return QVariant::fromValue((parameter.flags() & ParameterFlags::ShowAsSwitch) != 0);
        case ListRole:
        {
            if(parameter.flags() & ParameterFlags::ShowAsList)
            {
                if(auto iterator = valueList_.find(row); iterator != valueList_.end())
                {
                    return QVariant::fromValue(const_cast<PluginParameterValueListModel*>(&(iterator->second)));
                }
                else
                {
                    auto [inserted, succeeded] = valueList_.insert({row, PluginParameterValueListModel(&parameter)});
                    if(succeeded)
                    {
                        return QVariant::fromValue(const_cast<PluginParameterValueListModel*>(&(inserted->second)));
                    }
                }
            }
            else
            {
                return QVariant::fromValue(static_cast<QObject*>(nullptr));
            }
        }
        default:
            return QVariant();
        }
    }
}

bool PluginParameterListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    using namespace Musec::Audio::Plugin;
    int row = index.row();
    if(row >= 0 && row < parameterCount())
    {
        auto& parameter = plugin_->parameter(row);
        switch(role)
        {
        case ValueRole:
            parameter.setValue(value.value<double>());
            return true;
        default:
            break;
        }
    }
    return false;
}

RoleNamesType PluginParameterListModel::roleNames() const
{
    return roleNames_;
}
}