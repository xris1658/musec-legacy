#include "FileListModel.hpp"

#include <utility>

namespace Musec::Model
{

FileListModel::FileListModel(QObject* parent):
    QAbstractListModel(parent)
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::NameRole] = "name";
    roleNames_[RoleNames::PathRole] = "path";
}

FileListModel::~FileListModel()
{

}

void FileListModel::setList(const FileListModel::List& list)
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

void FileListModel::setList(FileListModel::List&& list)
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

FileListModel::List FileListModel::getList() const
{
    return list_;
}

int FileListModel::itemCount() const
{
    return list_.size();
}

constexpr int FileListModel::columnSize() noexcept
{
    return RoleNames::RoleEnd - Qt::UserRole;
}

int FileListModel::rowCount(const QModelIndex&) const
{
    return itemCount();
}

int FileListModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant FileListModel::data(const QModelIndex& index, int role) const
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

QString FileListModel::getPathOfIndex(int index) const
{
    return std::get<RoleNames::PathRole - Qt::UserRole>(list_[index]);
}

RoleNamesType FileListModel::roleNames() const
{
    return roleNames_;
}
}
