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
    list_ = list;
}

void FileListModel::setList(FileListModel::List&& list)
{
    list_ = std::move(list);
}

FileListModel::List FileListModel::getList() const
{
    return list_;
}

int FileListModel::itemCount() const
{
    return list_.size();
}

constexpr int FileListModel::columnSize()
{
    return std::tuple_size_v<Item>;
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
