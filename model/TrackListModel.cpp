#include "TrackListModel.hpp"

#include "base/Base.hpp"
#include "controller/AudioEngineController.hpp"

#include <QColor>

Musec::Model::TrackListModel::TrackListModel(QObject* parent):
    QAbstractListModel(parent),
    project_(Musec::Controller::AudioEngineController::AppProject())
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::NameRole] = "trackname";
    roleNames_[RoleNames::TypeRole] = "type";
    roleNames_[RoleNames::ColorRole] = "trackcolor";
    roleNames_[RoleNames::HeightRole] = "trackheight";
    roleNames_[RoleNames::MuteRole] = "mute";
    roleNames_[RoleNames::SoloRole] = "solo";
    roleNames_[RoleNames::InvertPhaseRole] = "invertPhase";
    roleNames_[RoleNames::ArmRecordingRole] = "armRecording";
}

Musec::Model::TrackListModel::~TrackListModel()
{

}

constexpr int Musec::Model::TrackListModel::columnSize()
{
    return RoleNames::kRoleNamesCount - Qt::UserRole;
}

int Musec::Model::TrackListModel::trackCount() const noexcept
{
    return project_.trackCount();
}

int Musec::Model::TrackListModel::rowCount(const QModelIndex&) const
{
    return project_.trackCount();
}

int Musec::Model::TrackListModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant Musec::Model::TrackListModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= trackCount())
    {
        return QVariant();
    }
    auto track = project_[row];
    auto information = track.track->getTrackInformation();
    switch(role)
    {
    case RoleNames::NameRole:
        return QVariant::fromValue(information.name);
    case RoleNames::TypeRole:
        return QVariant::fromValue(Musec::Base::underlyingValue(track.track->trackType()));
    case RoleNames::ColorRole:
        return QVariant::fromValue(information.color);
    case RoleNames::HeightRole:
        return QVariant::fromValue(information.height);
    case RoleNames::MuteRole:
        return QVariant::fromValue(static_cast<bool>(track.trackMute));
    case RoleNames::SoloRole:
        return QVariant::fromValue(static_cast<bool>(track.trackSolo));
    case RoleNames::InvertPhaseRole:
        return QVariant::fromValue(static_cast<bool>(track.trackInvertPhase));
    case RoleNames::ArmRecordingRole:
        return QVariant::fromValue(static_cast<bool>(track.trackArmRecording));
    default:
        return QVariant();
    }
}

bool Musec::Model::TrackListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    bool ret = false;
    int row = index.row();
    if(row < 0 || row >= trackCount())
    {
        ret = false;
    }
    else
    {
        auto trackRef = project_.at(row);
        auto information = trackRef.track->getTrackInformation();
        switch(role)
        {
        case RoleNames::NameRole:
            information.name = value.toString();
            trackRef.track->setTrackInformation(std::move(information));
            ret = true;
            break;
        case RoleNames::TypeRole:
            ret = false;
            break;
        case RoleNames::ColorRole:
            information.color = value.value<QColor>();
            trackRef.track->setTrackInformation(std::move(information));
            ret = true;
            break;
        case RoleNames::HeightRole:
            information.height = value.value<int>();
            trackRef.track->setTrackInformation(std::move(information));
            ret = true;
            break;
        case RoleNames::MuteRole:
            trackRef.trackMute = value.value<bool>();
            ret = true;
            break;
        case RoleNames::SoloRole:
            trackRef.trackSolo = value.value<bool>();
            ret = true;
            break;
        case RoleNames::InvertPhaseRole:
            trackRef.trackInvertPhase = value.value<bool>();
            ret = true;
            break;
        case RoleNames::ArmRecordingRole:
            trackRef.trackArmRecording = value.value<bool>();
            ret = true;
            break;
        }
    }
    if(ret)
    {
        dataChanged(index, index, QVector<int>(1, role));
    }
    return ret;
}

void Musec::Model::TrackListModel::insertTrack(int index, const Musec::Entities::CompleteTrack& track)
{
    emit beginInsertRows(QModelIndex(), index, index);
    project_.insertTrack(index, track);
    emit endInsertRows();
}

void Musec::Model::TrackListModel::appendTrack(const Musec::Entities::CompleteTrack& track)
{
    insertTrack(trackCount(), track);
}

void Musec::Model::TrackListModel::removeTrack(int index)
{
    emit beginRemoveRows(QModelIndex(), index, index);
    project_.eraseTrack(index);
    emit endRemoveRows();
}

Musec::Model::RoleNamesType Musec::Model::TrackListModel::roleNames() const
{
    return roleNames_;
}

Qt::ItemFlags Musec::Model::TrackListModel::flags(const QModelIndex& index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemFlag::ItemIsEditable;
}
