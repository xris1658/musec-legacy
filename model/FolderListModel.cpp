#include "FolderListModel.hpp"

#include <utility>

namespace Musec::Model
{

FolderListModel::FolderListModel(QObject* parent):
    QAbstractListModel(parent)
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::NameRole] = "name";
    roleNames_[RoleNames::PathRole] = "path";
}

FolderListModel::~FolderListModel()
{

}

void FolderListModel::setList(const FolderListModel::List& list)
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

void FolderListModel::setList(FolderListModel::List&& list)
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

FolderListModel::List FolderListModel::getList() const
{
    return list_;
}

int FolderListModel::itemCount() const
{
    return list_.count();
}

constexpr int FolderListModel::columnSize() noexcept
{
    return RoleNames::RoleEnd - Qt::UserRole;
}

int FolderListModel::rowCount(const QModelIndex&) const
{
    return itemCount();
}

int FolderListModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant FolderListModel::data(const QModelIndex& index, int role) const
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
    case RoleNames::PathRole:
        return QVariant::fromValue(std::get<RoleNames::PathRole - Qt::UserRole>(list_[row]));
    default:
        return QVariant();
    }
}

QString FolderListModel::getPathOfIndex(int index) const
{
    return std::get<RoleNames::PathRole - Qt::UserRole>(list_[index]);
}

RoleNamesType FolderListModel::roleNames() const
{
    return roleNames_;
}
}
