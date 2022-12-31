#include "TempoAutomationModel.hpp"

namespace Musec::Model
{
TempoAutomationModel::TempoAutomationModel(QObject* parent): QAbstractListModel(parent)
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::TimeRole] = "time";
    roleNames_[RoleNames::ValueRole] = "value";
    roleNames_[RoleNames::CurveRole] = "curve";
}

TempoAutomationModel::~TempoAutomationModel()
{

}

using TempoAutomation = TempoAutomationModel::TempoAutomation;

const TempoAutomation& TempoAutomationModel::getTempoAutomation() const
{
    return automation_;
}

void TempoAutomationModel::setTempoAutomation(const TempoAutomation& automation)
{
    auto oldRowCount = automation_.pointCount();
    automation_ = automation;
    automationSet(oldRowCount, automation_.pointCount());
}

void TempoAutomationModel::setTempoAutomation(TempoAutomation&& automation)
{
    auto oldRowCount = automation_.pointCount();
    automation_ = std::move(automation);
    automationSet(oldRowCount, automation_.pointCount());
}

int TempoAutomationModel::count() const noexcept
{
    return automation_.pointCount();
}

bool TempoAutomationModel::empty() const noexcept
{
    return automation_.empty();
}

constexpr int TempoAutomationModel::columnSize() noexcept
{
    return RoleNames::RoleEnd - Qt::UserRole;
}

int TempoAutomationModel::timeOfPoint(int index) const
{
    return automation_[index].time();
}

double TempoAutomationModel::valueOfPoint(int index) const
{
    return automation_[index].value();
}

double TempoAutomationModel::curveOfPoint(int index) const
{
    return automation_[index].curve();
}

double TempoAutomationModel::minValue() const noexcept
{
    return automation_.minValue();
}

double TempoAutomationModel::maxValue() const noexcept
{
    return automation_.maxValue();
}

int TempoAutomationModel::pointCountAtTime(int time) const
{
    return automation_.pointCountAtTime(time);
}

int TempoAutomationModel::firstPointIndexAtTime(int time) const
{
    return automation_.firstPointIndexAtTime(time);
}

double TempoAutomationModel::valueAtTime(int time) const
{
    return automation_(time);
}

int TempoAutomationModel::insertPoint(int time, double value, double curve, int indexInEqualTimePoint)
{
    auto ret = automation_.insertPoint({time, value, curve}, indexInEqualTimePoint);
    beginInsertRows(QModelIndex(), ret, ret);
    endInsertRows();
    return ret;
}

void TempoAutomationModel::deletePoint(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    automation_.deletePoint(index);
    endRemoveRows();
}

void TempoAutomationModel::setValueOfPoint(int index, double value)
{
    auto modelIndex = this->index(index);
    automation_.setValueOfPoint(index, value);
    dataChanged(modelIndex, modelIndex, {RoleNames::ValueRole});
}

int TempoAutomationModel::setTimeOfPoint(int index, int time, int indexInEqualTimePoint)
{
    auto ret = automation_.movePoint(index, time, indexInEqualTimePoint);
    if(ret != index)
    {
        beginMoveRows(QModelIndex(), index, index, QModelIndex(), ret);
    }
    auto modelIndex = this->index(ret);
    dataChanged(modelIndex, modelIndex, {RoleNames::TimeRole});
    return ret;
}

void TempoAutomationModel::setCurveOfPoint(int index, double curve)
{
    auto modelIndex = this->index(index);
    automation_.setCurveOfPoint(index, curve);
    dataChanged(modelIndex, modelIndex, {RoleNames::CurveRole});
}

void TempoAutomationModel::clearPoints()
{
    auto count = this->count();
    automation_.clearPoints();
    beginRemoveRows(QModelIndex(), 0, count - 1);
}

int TempoAutomationModel::rowCount(const QModelIndex&) const
{
    return count();
}

int TempoAutomationModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant TempoAutomationModel::data(const QModelIndex& index, int role) const
{
    auto row = index.row();
    if(row < 0 || row >= count())
    {
        return QVariant();
    }
    switch(role)
    {
    case RoleNames::TimeRole:
        return QVariant::fromValue(automation_[row].time());
    case RoleNames::ValueRole:
        return QVariant::fromValue(automation_[row].value());
    case RoleNames::CurveRole:
        return QVariant::fromValue(automation_[row].curve());
    default:
        return QVariant();
    }
}

bool TempoAutomationModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    auto row = index.row();
    if (row < 0 || row >= count())
    {
        return false;
    }
    switch (role)
    {
    case RoleNames::TimeRole:
        return false;
    case RoleNames::ValueRole:
        setValueOfPoint(row, value.value<double>());
        return true;
    case RoleNames::CurveRole:
        setCurveOfPoint(row, value.value<double>());
        return true;
    default:
        return false;
    }
}

RoleNamesType TempoAutomationModel::roleNames() const
{
    return roleNames_;
}

void TempoAutomationModel::automationSet(int oldRowCount, int newRowCount)
{
    beginRemoveRows(QModelIndex(), 0, oldRowCount - 1);
    endRemoveRows();
    beginInsertRows(QModelIndex(), 0, newRowCount - 1);
    endInsertRows();
}
}