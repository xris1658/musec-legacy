#include "PluginParameterValueListModel.hpp"

namespace Musec::Model
{
PluginParameterValueListModel::PluginParameterValueListModel(Musec::Audio::Plugin::IParameter* parameter, QObject* parent):
    QAbstractListModel(parent),
    parameter_(parameter)
{
}

PluginParameterValueListModel::PluginParameterValueListModel(const PluginParameterValueListModel& rhs):
    QAbstractListModel(rhs.parent()),
    parameter_(rhs.parameter_)
{
    //
}

PluginParameterValueListModel::~PluginParameterValueListModel()
{

}

int PluginParameterValueListModel::valueCount() const
{
    return std::round((parameter_->maxValue() - parameter_->minValue()) / parameter_->stepSize() + 1);
}

constexpr int PluginParameterValueListModel::columnSize() noexcept
{
    return RoleNames::RoleEnd - Qt::UserRole;
}

int PluginParameterValueListModel::rowCount(const QModelIndex&) const
{
    return valueCount();
}

int PluginParameterValueListModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant PluginParameterValueListModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < valueCount())
    {
        switch(role)
        {
        case ValueRole:
            return QVariant::fromValue(parameter_->minValue() + parameter_->stepSize() * row);
        case TextRole:
            return QVariant::fromValue(parameter_->valueToString(parameter_->minValue() + parameter_->stepSize() * row));
        default:
            return QVariant();
        }
    }
    return QVariant();
}

RoleNamesType PluginParameterValueListModel::roleNames() const
{
    static const RoleNamesType ret =
    {
        std::make_pair(RoleNames::ValueRole, "value"),
        std::make_pair(RoleNames::TextRole, "text")
    };
    return ret;
}
}