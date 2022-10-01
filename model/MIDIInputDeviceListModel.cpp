#include "MIDIInputDeviceListModel.hpp"

#include <utility>

namespace Musec::Model
{
MIDIInputDeviceListModel::MIDIInputDeviceListModel(QObject* parent): QAbstractListModel(parent)
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::NameRole] = "name";
    roleNames_[RoleNames::IdRole] = "id";
}

MIDIInputDeviceListModel::~MIDIInputDeviceListModel()
{

}

void MIDIInputDeviceListModel::setList(const MIDIInputDeviceListModel::List& list)
{
    if(!list_.empty())
    {
        beginRemoveRows(QModelIndex(), 0, itemCount() - 1);
        endRemoveRows();
    }
    list_ = list;
    if(!list_.empty())
    {
        beginInsertRows(QModelIndex(), 0, list_.size() - 1);
        endInsertRows();
    }
}

void MIDIInputDeviceListModel::setList(MIDIInputDeviceListModel::List&& list)
{
    if(!list_.empty())
    {
        beginRemoveRows(QModelIndex(), 0, itemCount() - 1);
        endRemoveRows();
    }
    list_ = std::move(list);
    if(!list_.empty())
    {
        beginInsertRows(QModelIndex(), 0, list_.size() - 1);
        endInsertRows();
    }
}

MIDIInputDeviceListModel::List MIDIInputDeviceListModel::getList() const
{
    return list_;
}

int MIDIInputDeviceListModel::itemCount() const
{
    return list_.count();
}

constexpr int MIDIInputDeviceListModel::columnSize()
{
    return std::tuple_size_v<Item>;
}

bool MIDIInputDeviceListModel::empty() const noexcept
{
    return list_.empty();
}

int MIDIInputDeviceListModel::rowCount(const QModelIndex&) const
{
    return itemCount();
}

int MIDIInputDeviceListModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant MIDIInputDeviceListModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= itemCount())
    {
        return QVariant();
    }
    switch(role)
    {
    case RoleNames::NameRole:
        return QVariant::fromValue(std::get<RoleNames::NameRole - Qt::UserRole>(list_[row]));
    case RoleNames::IdRole:
        return QVariant::fromValue(std::get<RoleNames::IdRole - Qt::UserRole>(list_[row]));
    default:
        return QVariant();
    }
}

RoleNamesType MIDIInputDeviceListModel::roleNames() const
{
    return roleNames_;
}
}