#include "AudioTrackSequenceModel.hpp"

namespace Musec::Model
{
AudioTrackSequenceModel::AudioTrackSequenceModel(QObject* parent):
    QAbstractListModel(parent)
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::StartTimeRole] = "startTime";
    roleNames_[RoleNames::EndTimeRole] = "endTime";
    roleNames_[RoleNames::ClipRole] = "clip";
    roleNames_[RoleNames::ClipStartsAtRole] = "clipStartsAt";
}

AudioTrackSequenceModel::~AudioTrackSequenceModel()
{
    //
}

const Musec::Audio::Arrangement::AudioTrackSequence& AudioTrackSequenceModel::getTrackSequence() const
{
    return trackSequence_;
}

Musec::Audio::Arrangement::AudioTrackSequence& AudioTrackSequenceModel::getTrackSequence()
{
    return trackSequence_;
}

void AudioTrackSequenceModel::setTrackSequence(const Audio::Arrangement::AudioTrackSequence& trackSequence)
{
    trackSequence_ = trackSequence;
}

void AudioTrackSequenceModel::setTrackSequence(Audio::Arrangement::AudioTrackSequence&& trackSequence)
{
    trackSequence_ = std::move(trackSequence);
}

int AudioTrackSequenceModel::clipCount() const
{
    return trackSequence_.size();
}

constexpr int AudioTrackSequenceModel::columnSize()
{
    return RoleNames::RoleNameCount - Qt::UserRole;
}

int AudioTrackSequenceModel::rowCount(const QModelIndex&) const
{
    return clipCount();
}

int AudioTrackSequenceModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant AudioTrackSequenceModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row > rowCount(index))
    {
        return QVariant();
    }
    switch(role)
    {
    case StartTimeRole:
        return QVariant::fromValue(trackSequence_[row].startTime_);
    case EndTimeRole:
        return QVariant::fromValue(trackSequence_[row].endTime_);
    case ClipRole:
        return QVariant::fromValue(trackSequence_[row].clip_.get());
    case ClipStartsAtRole:
        return QVariant::fromValue(trackSequence_[row].clipStartsAt_);
    default:
        return QVariant();
    }
}

RoleNamesType AudioTrackSequenceModel::roleNames() const
{
    return roleNames_;
}
}