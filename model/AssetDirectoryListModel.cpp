#include "AssetDirectoryListModel.hpp"

#include <utility>

namespace Musec::Model
{

AssetDirectoryListModel::AssetDirectoryListModel(QObject* parent):
    QAbstractListModel(parent)
{
}

AssetDirectoryListModel::~AssetDirectoryListModel()
{

}

void AssetDirectoryListModel::setList(const QList<Item>& list)
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

void AssetDirectoryListModel::setList(QList<Item>&& list)
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

QList<AssetDirectoryListModel::Item> AssetDirectoryListModel::getList() const
{
    return list_;
}

int AssetDirectoryListModel::itemCount() const
{
    return list_.count();
}

constexpr int AssetDirectoryListModel::columnSize() noexcept
{
    return RoleNames::RoleEnd - Qt::UserRole;
}

void AssetDirectoryListModel::append(const Item& item)
{
    beginInsertRows(QModelIndex(), itemCount(), itemCount());
    list_.append(item);
    endInsertRows();
}

void AssetDirectoryListModel::remove(int id)
{
    auto itemCount = this->itemCount();
    for(int i = 0; i < itemCount; ++i)
    {
        if(std::get<0>(list_[i]) == id)
        {
            beginRemoveRows(QModelIndex(), i, i);
            list_.removeAt(i);
            endRemoveRows();
            return;
        }
    }
}

void AssetDirectoryListModel::rename(int id, const QString& name)
{
    auto itemCount = this->itemCount();
    for(int i = 0; i < itemCount; ++i)
    {
        if(std::get<Musec::Base::AssetDirectoryInfoField::FieldId>(list_[i]) == id)
        {
            std::get<Musec::Base::AssetDirectoryInfoField::FieldName>(list_[i]) = name;
            dataChanged(index(i), index(i), QVector<int>(1, RoleNames::NameRole));
            return;
        }
    }
}

int AssetDirectoryListModel::rowCount(const QModelIndex&) const
{
    return itemCount();
}

int AssetDirectoryListModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant AssetDirectoryListModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= itemCount())
    {
        return QVariant();
    }
    switch(role)
    {
    case RoleNames::IdRole:
        return QVariant::fromValue(std::get<RoleNames::IdRole - Qt::UserRole>(list_[row]));
    case RoleNames::DirectoryRole:
        return QVariant::fromValue(std::get<RoleNames::DirectoryRole - Qt::UserRole>(list_[row]));
    case RoleNames::NameRole:
        return QVariant::fromValue(std::get<RoleNames::NameRole - Qt::UserRole>(list_[row]));
    default:
        return QVariant();
    }
}

RoleNamesType AssetDirectoryListModel::roleNames() const
{
    static const RoleNamesType ret =
        {
            std::make_pair(RoleNames::IdRole, "directoryId"),
            std::make_pair(RoleNames::DirectoryRole, "directory"),
            std::make_pair(RoleNames::NameRole, "name")
        };
    return ret;
}
}
