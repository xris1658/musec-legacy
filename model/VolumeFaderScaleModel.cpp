#include "VolumeFaderScaleModel.hpp"

namespace Musec::Model
{
VolumeFaderScaleModel::VolumeFaderScaleModel(QObject* parent):
    QAbstractListModel(parent),
    maxDecibel_(6),
    minDecibel_(-144)
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::PositionRole] = "position";
    roleNames_[RoleNames::DecibelRole] = "decibel";
}

double VolumeFaderScaleModel::maxDecibel() const
{
    return maxDecibel_;
}

bool VolumeFaderScaleModel::setMaxDecibel(double maxDecibel)
{
    if(!container_.empty())
    {
        if(maxDecibel <= container_.front().second)
        {
            return false;
        }
    }
    maxDecibel_ = maxDecibel;
    maxDecibelChanged();
    return true;
}

double VolumeFaderScaleModel::minDecibel() const
{
    return minDecibel_;
}

bool VolumeFaderScaleModel::setMinDecibel(double minDecibel)
{
    if(!container_.empty())
    {
        if(minDecibel >= container_.back().second)
        {
            return false;
        }
    }
    minDecibel_ = minDecibel;
    minDecibelChanged();
    return true;
}

int VolumeFaderScaleModel::rowCount() const
{
    return container_.size();
}

constexpr int VolumeFaderScaleModel::columnSize() noexcept
{
    return RoleNames::RoleEnd - Qt::UserRole;
}

int VolumeFaderScaleModel::rowCount(const QModelIndex&) const
{
    return rowCount();
}

int VolumeFaderScaleModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant VolumeFaderScaleModel::data(const QModelIndex& index, int role) const
{
    if(int row = index.row(); row >= 0 && row < rowCount())
    {
        switch(role)
        {
        case RoleNames::PositionRole:
            return QVariant::fromValue(container_[row].first);
        case RoleNames::DecibelRole:
            return QVariant::fromValue(container_[row].second);
        default:
            return QVariant();
        }
    }
    return QVariant();
}

bool VolumeFaderScaleModel::insertPoint(double position, double decibel)
{
    if(decibel >= maxDecibel_ || decibel <= minDecibel_)
    {
        return false;
    }
    auto iterator = std::lower_bound(container_.begin(), container_.end(), position,
        [](const std::pair<double, double>& point, double position) { return point.first < position; });
    if(iterator == container_.end())
    {
        if(container_.empty() || container_.back().second < decibel)
        {
            auto insertIndex = std::distance(container_.begin(), iterator);
            beginInsertRows(QModelIndex(), insertIndex, insertIndex);
            container_.emplace(iterator, position, decibel);
            endInsertRows();
            return true;
        }
        return false;
    }
    if(iterator == container_.begin())
    {
        if(container_.front().second > decibel)
        {
            beginInsertRows(QModelIndex(), 0, 0);
            container_.emplace(iterator, position, decibel);
            endInsertRows();
            return true;
        }
        return false;
    }
    auto beforeInsertion = iterator - 1;
    if(beforeInsertion->second < decibel && iterator->second > decibel)
    {
        auto insertIndex = std::distance(container_.begin(), iterator);
        beginInsertRows(QModelIndex(), insertIndex, insertIndex);
        container_.emplace(iterator, position, decibel);
        endInsertRows();
        return true;
    }
    return false;
}

bool VolumeFaderScaleModel::removePoint(int index)
{
    if(index >= 0 && index < rowCount())
    {
        beginRemoveRows(QModelIndex(), index, index);
        container_.erase(container_.begin() + index);
        endRemoveRows();
        return true;
    }
    return false;
}

double VolumeFaderScaleModel::decibelFromPosition(double position)
{
    if(position <= 0.0)
    {
        return minDecibel_;
    }
    if(position >= 1.0)
    {
        return maxDecibel_;
    }
    if(container_.empty())
    {
        return minDecibel_ + (maxDecibel_ - minDecibel_) * position;
    }
    auto iterator = std::lower_bound(container_.begin(), container_.end(), position,
        [](const std::pair<double, double>& point, double position) { return point.first < position; });
    if(iterator == container_.end())
    {
        auto x1 = container_.back().first;
        auto y1 = container_.back().second;
        return y1 + (position - x1) * (maxDecibel_ - y1) / (1.0 - x1);
    }
    if(auto x2 = iterator->first; x2 == position)
    {
        return iterator->second;
    }
    else
    {
        auto y2 = iterator->second;
        if(iterator == container_.begin())
        {
            return minDecibel_ + (iterator->second - minDecibel_) * position / x2;
        }
        else
        {
            auto before = iterator - 1;
            auto x1 = before->first;
            auto y1 = before->second;
            return y1 + (y2 - y1) * (position - x1) / (x2 - x1);
        }
    }
}

double VolumeFaderScaleModel::positionFromDecibel(double decibel)
{
    if(decibel >= maxDecibel_)
    {
        return 1.0;
    }
    if(decibel <= minDecibel_)
    {
        return 0.0;
    }
    if(container_.empty())
    {
        return (decibel - minDecibel_) / (maxDecibel_ - minDecibel_);
    }
    auto iterator = std::lower_bound(container_.begin(), container_.end(), decibel,
        [](const std::pair<double, double>& point, double decibel) { return point.second < decibel; });
    if(iterator == container_.end())
    {
        auto x1 = container_.back().first;
        auto y1 = container_.back().second;
        return x1 + (1 - x1) * (decibel - y1) / (maxDecibel_ - y1);
    }
    if(auto y2 = iterator->second; y2 == decibel)
    {
        return iterator->first;
    }
    else
    {
        auto x2 = iterator->first;
        if(iterator == container_.begin())
        {
            auto y1 = minDecibel_;
            return x2 * (decibel - y1) / (y2 - y1);
        }
        else
        {
            auto before = iterator - 1;
            auto x1 = before->first;
            auto y1 = before->second;
            return x1 + (x2 - x1) * (decibel - y1) / (y2 - y1);
        }
    }
}

RoleNamesType VolumeFaderScaleModel::roleNames() const
{
    return roleNames_;
}
}