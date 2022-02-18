#include "PluginListModel.hpp"

#include <utility>
#include <tuple>

namespace Musec
{
namespace Model
{

PluginListModel::PluginListModel(QObject* parent): QAbstractListModel(parent)
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::IdRole] = "id";
    roleNames_[RoleNames::PathRole] = "path";
    roleNames_[RoleNames::UidRole] = "uid";
    roleNames_[RoleNames::NameRole] = "name";
    roleNames_[RoleNames::FormatRole] = "format";
    roleNames_[RoleNames::TypeRole] = "type";
}

PluginListModel::~PluginListModel()
{

}

void PluginListModel::setList(const PluginListModel::List& list)
{
    list_ = list;
}

void PluginListModel::setList(PluginListModel::List&& list)
{
    list_ = std::move(list);
}

PluginListModel::List PluginListModel::getList() const
{
    return list_;
}

int PluginListModel::itemCount() const
{
    return list_.count();
}

constexpr int PluginListModel::columnSize() const
{
    return std::tuple_size<Item>::value;
}

int PluginListModel::rowCount(const QModelIndex&) const
{
    return itemCount();
}

int PluginListModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant PluginListModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= itemCount())
    {
        return QVariant();
    }
    auto column = role - Qt::UserRole;
    if(column < 0 || column > columnSize())
    {
        return QVariant();
    }
    switch(role)
    {
    case RoleNames::IdRole:
        return QVariant::fromValue(std::get<RoleNames::IdRole - Qt::UserRole>(list_[row]));
    case RoleNames::PathRole:
        return QVariant::fromValue(std::get<RoleNames::PathRole - Qt::UserRole>(list_[row]));
    case RoleNames::UidRole:
        return QVariant::fromValue(std::get<RoleNames::UidRole - Qt::UserRole>(list_[row]));
    case RoleNames::NameRole:
        return QVariant::fromValue(std::get<RoleNames::NameRole - Qt::UserRole>(list_[row]));
    case RoleNames::FormatRole:
        return QVariant::fromValue(std::get<RoleNames::FormatRole - Qt::UserRole>(list_[row]));
    case RoleNames::TypeRole:
        return QVariant::fromValue(std::get<RoleNames::TypeRole - Qt::UserRole>(list_[row]));
    default:
        return QVariant();
    }
}

RoleNamesType PluginListModel::roleNames() const
{
    return roleNames_;
}
}
}
