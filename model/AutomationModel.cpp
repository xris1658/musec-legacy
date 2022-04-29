#include "AutomationModel.hpp"

#include <utility>

namespace Musec::Model
{
AutomationModel::AutomationModel(QObject* parent):
    QAbstractListModel(parent)
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::TimeRole] = "time";
    roleNames_[RoleNames::ValueRole] = "value";
    roleNames_[RoleNames::CurveRole] = "curve";
}

AutomationModel::~AutomationModel()
{
    //
}

void AutomationModel::setAutomation(const Automation& automation)
{
    if(automation_.empty())
    {
        beginRemoveRows(QModelIndex(), 0, automation_.pointCount() - 1);
        endRemoveRows();
    }
    automation_ = automation;
    if(!automation_.empty())
    {
        beginInsertRows(QModelIndex(), 0, automation_.pointCount() - 1);
        endInsertRows();
    }
}

void AutomationModel::setAutomation(Automation&& automation)
{
    if(automation_.empty())
    {
        beginRemoveRows(QModelIndex(), 0, automation_.pointCount() - 1);
        endRemoveRows();
    }
    automation_ = std::move(automation);
    if(!automation_.empty())
    {
        beginInsertRows(QModelIndex(), 0, automation_.pointCount() - 1);
        endInsertRows();
    }
}

const AutomationModel::Automation& AutomationModel::getAutomation() const
{
    return automation_;
}

int AutomationModel::pointCount() const
{
    return automation_.pointCount();
}

constexpr int AutomationModel::columnSize()
{
    return Musec::Audio::Base::AutomationPointWithCurveColumnCount;
}

int AutomationModel::rowCount(const QModelIndex&) const
{
    return pointCount();
}

int AutomationModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant AutomationModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= rowCount(index))
    {
        return QVariant();
    }
    switch(role)
    {
        case RoleNames::TimeRole:
            return QVariant::fromValue(automation_[row].time_);
        case RoleNames::ValueRole:
            return QVariant::fromValue(automation_[row].value_);
        case RoleNames::CurveRole:
            return QVariant::fromValue(automation_[row].curve_);
        default:
            return QVariant();
    }
}

RoleNamesType AutomationModel::roleNames() const
{
    return roleNames_;
}
}