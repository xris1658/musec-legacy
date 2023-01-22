#include "ASIODriverListModel.hpp"

#include <utility>

namespace Musec::Model
{
ASIODriverListModel::ASIODriverListModel(QObject* parent):
    QAbstractListModel(parent)
{
}

ASIODriverListModel::~ASIODriverListModel()
{

}

void ASIODriverListModel::setList(const List& list)
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

void ASIODriverListModel::setList(List&& list)
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

ASIODriverListModel::List ASIODriverListModel::getList() const
{
    return list_;
}

int ASIODriverListModel::itemCount() const
{
    return list_.size();
}

constexpr int ASIODriverListModel::columnSize() noexcept
{
    return RoleNames::RoleEnd - Qt::UserRole;
}

bool ASIODriverListModel::empty() const noexcept
{
    return list_.empty();
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
    static const RoleNamesType ret =
    {
        std::make_pair(RoleNames::NameRole, "name"),
        std::make_pair(RoleNames::CLSIDRole, "clsid")
    };
    return ret;
}
}
