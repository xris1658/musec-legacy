#include "AutomationModel.hpp"

namespace Musec::Model
{
AutomationModel::AutomationModel(QObject* parent) : QAbstractListModel(parent)
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::TimeRole] = "time";
    roleNames_[RoleNames::ValueRole] = "value";
    roleNames_[RoleNames::CurveRole] = "curve";
}

AutomationModel::~AutomationModel()
{

}

const Musec::Audio::Base::Automation& AutomationModel::getAutomation() const
{
    return automation_;
}

void AutomationModel::setAutomation(const Musec::Audio::Base::Automation& automation)
{
    automation_ = automation;
    // TODO: 发信号告知自动化改变
}

void AutomationModel::setAutomation(Musec::Audio::Base::Automation&& automation)
{
    automation_ = std::move(automation);
    // TODO: 发信号告知自动化改变
}

int AutomationModel::count() const noexcept
{
    return automation_.pointCount();
}

bool AutomationModel::empty() const noexcept
{
    return automation_.empty();
}

constexpr int AutomationModel::columnSize() noexcept
{
    // TODO: 将此数字移入 Automation 所在文件中
    return 3;
}

double AutomationModel::timeOfPoint(int index) const
{
    return automation_[index].time();
}

double AutomationModel::valueOfPoint(int index) const
{
    return automation_[index].value();
}

double AutomationModel::curveOfPoint(int index) const
{
    return automation_[index].curve();
}

double AutomationModel::minValue() const noexcept
{
    return automation_.minValue();
}

double AutomationModel::maxValue() const noexcept
{
    return automation_.maxValue();
}

int AutomationModel::pointCountAtTime(int time) const
{
    return automation_.pointCountAtTime(time);
}

int AutomationModel::firstPointIndexAtTime(int time) const
{
    return automation_.firstPointIndexAtTime(time);
}

double AutomationModel::valueAtTime(int time) const
{
    return automation_(time);
}

int AutomationModel::insertPoint(int time, double value, double curve, int indexInEqualTimePoint)
{
    auto ret = automation_.insertPoint({time, value, curve}, indexInEqualTimePoint);
    beginInsertRows(QModelIndex(), ret, ret);
    endInsertRows();
    return ret;
}

void AutomationModel::deletePoint(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    automation_.deletePoint(index);
    endRemoveRows();
}

void AutomationModel::setValueOfPoint(int index, double value)
{
    auto modelIndex = this->index(index);
    automation_.setValueOfPoint(index, value);
    dataChanged(modelIndex, modelIndex, {RoleNames::ValueRole});
}

int AutomationModel::setTimeOfPoint(int index, int time, int indexInEqualTimePoint)
{
    auto ret = automation_.setTimeOfPoint(index, time, indexInEqualTimePoint);
    if(ret != index)
    {
        beginMoveRows(QModelIndex(), index, index, QModelIndex(), ret);
        // 移动行是否会使 View 同步更新数据？
    }
    auto modelIndex = this->index(ret);
    dataChanged(modelIndex, modelIndex, {RoleNames::TimeRole});
    return ret;
}

void AutomationModel::setCurveOfPoint(int index, double curve)
{
    auto modelIndex = this->index(index);
    automation_.setCurveOfPoint(index, curve);
    dataChanged(modelIndex, modelIndex, {RoleNames::CurveRole});
}

void AutomationModel::clearPoints()
{
    auto count = this->count();
    automation_.clearPoints();
    beginRemoveRows(QModelIndex(), 0, count - 1);
}

int AutomationModel::rowCount(const QModelIndex&) const
{
    return count();
}

int AutomationModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant AutomationModel::data(const QModelIndex& index, int role) const
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

bool AutomationModel::setData(const QModelIndex& index, const QVariant& value, int role)
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

RoleNamesType AutomationModel::roleNames() const
{
    return roleNames_;
}
}
