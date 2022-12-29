#include "PluginParameterValueListModel.hpp"

namespace Musec::Model
{
PluginParameterValueListModel::PluginParameterValueListModel(Musec::Audio::Plugin::IParameter* parameter, QObject* parent):
    QAbstractListModel(parent),
    parameter_(parameter)
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::ValueRole] = "value";
    roleNames_[RoleNames::TextRole] = "text";
}

PluginParameterValueListModel::PluginParameterValueListModel(const PluginParameterValueListModel& rhs):
    QAbstractListModel(rhs.parent()),
    parameter_(rhs.parameter_),
    roleNames_(rhs.roleNames_)
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

constexpr int PluginParameterValueListModel::columnSize()
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
    return roleNames_;
}
}