#include "ASIODriverListModel.hpp"

#include <utility>

namespace Musec::Model
{
ASIODriverListModel::ASIODriverListModel(QObject* parent):
    QAbstractListModel(parent)
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::NameRole] = "name";
    roleNames_[RoleNames::CLSIDRole] = "clsid";
}

ASIODriverListModel::~ASIODriverListModel()
{

}

void ASIODriverListModel::setList(const List& list)
{
    list_ = list;
}

void ASIODriverListModel::setList(List&& list)
{
    list_ = std::move(list);
}

ASIODriverListModel::List ASIODriverListModel::getList() const
{
    return list_;
}

int ASIODriverListModel::itemCount() const
{
    return list_.count();
}

constexpr int ASIODriverListModel::columnSize()
{
    return std::tuple_size_v<Item>;
}

int ASIODriverListModel::rowCount(const QModelIndex&) const
{
    return itemCount();
}

int ASIODriverListModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant ASIODriverListModel::data(const QModelIndex& index, int role) const
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
    case RoleNames::CLSIDRole:
        return QVariant::fromValue(std::get<RoleNames::CLSIDRole - Qt::UserRole>(list_[row]));
    default:
        return QVariant();
    }
}

RoleNamesType ASIODriverListModel::roleNames() const
{
    return roleNames_;
}
}
