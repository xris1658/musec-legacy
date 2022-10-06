#include "ASIOChannelInfoListModel.hpp"

namespace Musec::Model
{
ASIOChannelInfoListModel::ASIOChannelInfoListModel(QObject* parent):
    QAbstractListModel(parent),
    channelInfoList_(nullptr),
    channelCount_(0)
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::ChannelIndexRole] = "channel_index";
    roleNames_[RoleNames::IsInputRole] = "is_input";
    roleNames_[RoleNames::IsActiveRole] = "is_active";
    roleNames_[RoleNames::GroupRole] = "group";
    roleNames_[RoleNames::SampleTypeRole] = "sample_type";
    roleNames_[RoleNames::NameRole] = "name";
}

ASIOChannelInfoListModel::~ASIOChannelInfoListModel()
{

}

void ASIOChannelInfoListModel::setList(ASIOChannelInfo* const channelInfoList, int channelCount)
{
    channelInfoList_ = channelInfoList;
    channelCount_ = channelCount;
}

ASIOChannelInfo* ASIOChannelInfoListModel::getList() const
{
    return channelInfoList_;
}

int ASIOChannelInfoListModel::itemCount() const
{
    return channelCount_;
}

constexpr int ASIOChannelInfoListModel::columnSize()
{
    return RoleNames::RoleEnd - Qt::UserRole;
}

bool ASIOChannelInfoListModel::empty() const noexcept
{
    return channelCount_ == 0;
}

int ASIOChannelInfoListModel::rowCount(const QModelIndex&) const
{
    return itemCount();
}

int ASIOChannelInfoListModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant ASIOChannelInfoListModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= itemCount())
    {
        return QVariant();
    }
    switch(role)
    {
    case ChannelIndexRole:
        return QVariant::fromValue(channelInfoList_[row].channel);
    case IsInputRole:
        return QVariant::fromValue(channelInfoList_[row].isInput);
    case IsActiveRole:
        return QVariant::fromValue(channelInfoList_[row].isActive);
    case GroupRole:
        return QVariant::fromValue(channelInfoList_[row].channelGroup);
    case NameRole:
        return QVariant::fromValue(QString(channelInfoList_[row].name));
    default:
        return QVariant();
    }
}

RoleNamesType ASIOChannelInfoListModel::roleNames() const
{
    return roleNames_;
}
}