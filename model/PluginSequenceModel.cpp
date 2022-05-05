#include "PluginSequenceModel.hpp"

namespace Musec
{
namespace Model
{
PluginSequenceModel::PluginSequenceModel(const PluginSequenceModel::List& list)
{
    setList(list);
}

PluginSequenceModel::PluginSequenceModel(PluginSequenceModel::List&& list)
{
    setList(std::move(list));
}

PluginSequenceModel::PluginSequenceModel(QObject* parent): QAbstractListModel(parent)
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::ValidRole] = "valid";
    roleNames_[RoleNames::ActivatedRole] = "activated";
    roleNames_[RoleNames::NameRole] = "name";
    roleNames_[RoleNames::SidechainExistRole] = "sidechain_exist";
    roleNames_[RoleNames::SidechainEnabledRole] = "sidechain_enabled";
}

PluginSequenceModel::~PluginSequenceModel()
{

}

const PluginSequenceModel::List& PluginSequenceModel::getList() const
{
    return list_;
}

void PluginSequenceModel::setList(const PluginSequenceModel::List& list)
{
    beginResetModel();
    list_ = list;
    endResetModel();
}

void PluginSequenceModel::setList(PluginSequenceModel::List&& list)
{
    beginResetModel();
    list_ = std::move(list);
    endResetModel();
}

int PluginSequenceModel::itemCount() const
{
    return list_.size();
}

constexpr int PluginSequenceModel::columnSize()
{
    return RoleNames::RoleNamesEnd - Qt::UserRole;
}

void PluginSequenceModel::insert(PluginSequenceModel::Item* plugin, int index)
{
    if(index < 0)
    {
        return;
    }
    if(index > list_.size())
    {
        index = list_.size();
    }
    beginInsertRows(this->index(0), index, index);
    auto list = list_;
    list.emplace(list.begin() + index, plugin);
    list_ = list;
    endInsertRows();
}

void PluginSequenceModel::move(int from, int to)
{
    if(from < 0 || from >= list_.size() || to < 0 || to >= list_.size())
    {
        return;
    }
    beginMoveRows(this->index(0), from, from, this->index(0), to);
    auto list = list_;
    auto delta = to - from;
    if(delta > 0)
    {
        for(int i = 1; i < delta; ++i)
        {
            std::swap(list[from + i], list[from + i + 1]);
        }
    }
    else
    {
        for(int i = 0; i > delta; --i)
        {
            std::swap(list[from - i], list[from - i - 1]);
        }
    }
    endMoveRows();
}

void PluginSequenceModel::remove(int index)
{
    if(index < 0 || index >= list_.size())
    {
        return;
    }
    beginRemoveRows(this->index(0), index, index);
    auto list = list_;
    list.erase(list.begin() + index);
    list_ = list;
    endRemoveRows();
}

int PluginSequenceModel::rowCount(const QModelIndex&) const
{
    return itemCount();
}

int PluginSequenceModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant PluginSequenceModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= columnSize())
    {
        switch(role)
        {
        case RoleNames::ValidRole:
            return QVariant::fromValue(list_[row]->valid());
        case RoleNames::ActivatedRole:
            return QVariant::fromValue(list_[row]->isEnabled());
        case RoleNames::NameRole:
            return QVariant::fromValue(list_[row]->getName());
        case RoleNames::SidechainExistRole:
            return QVariant::fromValue(list_[row]->isSidechainExist());
        case RoleNames::SidechainEnabledRole:
            return QVariant::fromValue(list_[row]->isSidechainEnabled());
        default:
            return QVariant();
        }
    }
}

bool PluginSequenceModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    int row = index.row();
    if(row < 0 || row >= rowCount(index))
    {
        return false;
    }
    switch(role)
    {
    case ValidRole:
        return false;
    case ActivatedRole:
        list_[row]->setEnabled(value.value<bool>());
        return true;
    case NameRole:
        return false;
    case SidechainExistRole:
        list_[row]->setSidechainExist(value.value<bool>());
        return true;
    case SidechainEnabledRole:
        list_[row]->setSidechainEnabled(value.value<bool>());
        return true;
    default:
        return false;
    }
}

RoleNamesType PluginSequenceModel::roleNames() const
{
    return roleNames_;
}
}
}
