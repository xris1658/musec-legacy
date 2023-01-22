#include "AutomationModel.hpp"

namespace Musec::Model
{
AutomationModel::AutomationModel(QObject* parent) : QAbstractListModel(parent)
{
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
    automationAboutToSet();
    automation_ = automation;
    automationSet();
}

void AutomationModel::setAutomation(Musec::Audio::Base::Automation&& automation)
{
    automationAboutToSet();
    automation_ = std::move(automation);
    automationSet();
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
    return RoleNames::RoleEnd - Qt::UserRole;
}

int AutomationModel::timeOfPoint(int index) const
{
    if(index < 0 || index >= count())
    {
        throw std::out_of_range("Out of range. If this function is not called by your own C++ codes, check your QML code.");
    }
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
    auto newIndex = automation_.ifSetTimeOfPoint(index, time, indexInEqualTimePoint);
    if(newIndex == index)
    {
        automation_.movePoint(index, time, indexInEqualTimePoint);
        dataChanged(this->index(index), this->index(index), {RoleNames::TimeRole});
    }
    else
    {
        beginResetModel();

        // // Curves need to be updated
        // beginMoveRows(this->index(0), index, index, this->index(0),
        //     (newIndex > index? newIndex + 1: newIndex));
        automation_.movePoint(index, time, indexInEqualTimePoint);
        // endMoveRows();
        // dataChanged(this->index(std::min(index, int(newIndex))),
        //     this->index(std::max(index, int(newIndex)))/*, QVector<int>()*/);

        endResetModel();
    }
    return newIndex;
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
        setTimeOfPoint(row, value.value<int>(), 0);
        return true;
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
    static const RoleNamesType ret =
    {
        std::make_pair(RoleNames::TimeRole, "time"),
        std::make_pair(RoleNames::ValueRole, "value"),
        std::make_pair(RoleNames::CurveRole, "curve")
    };
    return ret;
}

void AutomationModel::automationAboutToSet()
{
    auto oldRowCount = automation_.pointCount();
    if(oldRowCount != 0)
    {
        beginRemoveRows(QModelIndex(), 0, oldRowCount  -1);
        endRemoveRows();
    }
}

void AutomationModel::automationSet()
{
    auto newRowCount = automation_.pointCount();
    if(newRowCount != 0)
    {
        beginInsertRows(QModelIndex(), 0, newRowCount - 1);
        endInsertRows();
    }
}
}
