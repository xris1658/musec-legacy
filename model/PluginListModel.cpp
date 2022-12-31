#include "PluginListModel.hpp"

#include <QByteArray>

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
    roleNames_[RoleNames::NameRole] = "name";
    roleNames_[RoleNames::FormatRole] = "format";
    roleNames_[RoleNames::TypeRole] = "type";
}

PluginListModel::~PluginListModel()
{
}

void PluginListModel::setList(const PluginListModel::List& list)
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

void PluginListModel::setList(PluginListModel::List&& list)
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

PluginListModel::List PluginListModel::getList() const
{
    return list_;
}

int PluginListModel::itemCount() const
{
    return list_.count();
}

constexpr int PluginListModel::columnSize()
{
    return RoleNames::RoleEnd - Qt::UserRole;
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
    using namespace Musec::Base;
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
        return QVariant::fromValue(std::get<PluginReadInfoField::ReadFieldId>(list_[row]));
    case RoleNames::NameRole:
        return QVariant::fromValue(std::get<PluginReadInfoField::ReadFieldName>(list_[row]));
    case RoleNames::FormatRole:
        return QVariant::fromValue(std::get<PluginReadInfoField::ReadFieldFormat>(list_[row]));
    case RoleNames::TypeRole:
        return QVariant::fromValue(std::get<PluginReadInfoField::ReadFieldType>(list_[row]));
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
