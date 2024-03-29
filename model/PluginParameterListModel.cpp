#include "PluginParameterListModel.hpp"

#include "audio/plugin/IParameter.hpp"

namespace Musec::Model
{
PluginParameterListModel::PluginParameterListModel(Musec::Audio::Plugin::IPlugin* plugin, QObject* parent):
    QAbstractListModel(parent),
    plugin_(plugin)
{
}

PluginParameterListModel::PluginParameterListModel(const PluginParameterListModel& rhs):
    QAbstractListModel(rhs.parent()),
    plugin_(rhs.plugin_)
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

constexpr int PluginParameterListModel::columnSize() noexcept
{
    return RoleNames::RoleEnd - Qt::UserRole;
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
    static const RoleNamesType ret =
    {
        std::make_pair(RoleNames::IdRole, "parameterId"),
        std::make_pair(RoleNames::NameRole, "name"),
        std::make_pair(RoleNames::ShortNameRole, "shortName"),
        std::make_pair(RoleNames::DiscreteRole, "discrete"),
        std::make_pair(RoleNames::PeriodicRole, "periodic"),
        std::make_pair(RoleNames::HiddenRole, "hidden"),
        std::make_pair(RoleNames::ReadonlyRole, "readonly"),
        std::make_pair(RoleNames::AutomatableRole, "automatable"),
        std::make_pair(RoleNames::MinValueRole, "minValue"),
        std::make_pair(RoleNames::MaxValueRole, "maxValue"),
        std::make_pair(RoleNames::DefaultValueRole, "defaultValue"),
        std::make_pair(RoleNames::ValueRole, "value"),
        std::make_pair(RoleNames::StepRole, "step"),
        std::make_pair(RoleNames::ShowAsListRole, "showAsList"),
        std::make_pair(RoleNames::ShowAsSwitchRole, "showAsSwitch"),
        std::make_pair(RoleNames::ListRole, "list")
    };
    return ret;
}
}