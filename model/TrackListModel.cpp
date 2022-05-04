#include "TrackListModel.hpp"

#include "audio/track/InstrumentTrack.hpp"
#include "base/Base.hpp"
#include "controller/AudioEngineController.hpp"
#include "entities/Plugin.hpp"
#include "model/PluginSequenceModel.hpp"
#include "audio/plugin/VST2Plugin.hpp"
#include "audio/plugin/VST3Plugin.hpp"
#include "ui/PluginWindow.hpp"

#include <QColor>
#include <QVector>

namespace Impl
{
template<typename SampleType>
Musec::Entities::Plugin getPlugin(const Musec::Audio::Plugin::IPlugin<SampleType>* plugin)
{
    auto ret = Musec::Entities::Plugin();
    if(plugin)
    {
        ret.setEnabled(!plugin->getBypass());
        ret.setName(plugin->getName());
        ret.setSidechainExist(false);
        ret.setSidechainEnabled(false);
    }
    return ret;
}
}

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
    roleNames_[RoleNames::InstrumentRole] = "instrument";
    roleNames_[RoleNames::PluginListRole] = "pluginList";
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
    case RoleNames::InstrumentRole:
    {
        if(track.track->trackType() != Musec::Audio::Track::kInstrumentTrack)
        {
            return QVariant();
        }
        auto plugin = std::static_pointer_cast<Musec::Audio::Track::InstrumentTrack>(track.track)->getInstrument().get();
        return QVariant::fromValue(new Musec::Entities::Plugin(Impl::getPlugin<double>(plugin)));
    }
    case RoleNames::PluginListRole:
    {
        const auto type = track.track->trackType();
        if(type == Musec::Audio::Track::kMIDITrack)
        {
            return QVariant();
        }
        else if(type == Musec::Audio::Track::kInstrumentTrack)
        {
            auto pluginSequence = std::static_pointer_cast<Musec::Audio::Track::InstrumentTrack>(track.track)->getAudioEffectPluginSequences()[0];
            auto pluginSequenceModel = new Musec::Model::PluginSequenceModel();
            QVector<Musec::Entities::Plugin*> list;
            list.reserve(pluginSequence.size());
            for(int i = 0; i < pluginSequence.size(); ++i)
            {
                list.append(new Musec::Entities::Plugin(Impl::getPlugin<double>(pluginSequence[i].get())));
            }
            return QVariant::fromValue(list);
        }
    }
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

void Musec::Model::TrackListModel::loadInstrument(int trackIndex, int pluginFormat, const QString& path, int pluginSubId)
{
    auto track = project_[trackIndex];
    if(track.track->trackType() != Musec::Audio::Track::TrackType::kInstrumentTrack)
    {
        return;
    }
    auto instrumentTrack = std::static_pointer_cast<Musec::Audio::Track::InstrumentTrack>(project_[trackIndex].track);
    if(pluginFormat == Musec::Base::PluginFormat::FormatVST3)
    {
        auto instrument = std::make_shared<Musec::Audio::Plugin::VST3Plugin<double>>(
            path, pluginSubId
        );
        instrumentTrack->setInstrument(instrument);
        try
        {
            instrument->initialize(Musec::Controller::AudioEngineController::getCurrentSampleRate(),
                                   Musec::Controller::AudioEngineController::getMaxBlockSize());
        }
        catch(...)
        {
            return;
        }
        Musec::UI::createNewPluginWindow(instrument);
        dataChanged(index(trackIndex), index(trackIndex), QVector<int>(1, RoleNames::InstrumentRole));
    }
    else if(pluginFormat == Musec::Base::PluginFormat::FormatVST2)
    {
        auto instrument = std::make_shared<Musec::Audio::Plugin::VST2Plugin<double>>(
            path, false, pluginSubId
        );
        instrumentTrack->setInstrument(instrument);
        instrument->initialize(Musec::Controller::AudioEngineController::getCurrentSampleRate(),
                               Musec::Controller::AudioEngineController::getMaxBlockSize());
        Musec::UI::createNewPluginWindow(instrument);
        dataChanged(index(trackIndex), index(trackIndex), QVector<int>(1, RoleNames::InstrumentRole));
    }
    else
    {
        return;
    }
}

void Musec::Model::TrackListModel::loadEffect(
    int trackIndex, int pluginFormat, const QString& path, int pluginSubId, int pluginIndex)
{
    auto track = project_[trackIndex];
    auto trackType = track.track->trackType();
    if(trackType == Musec::Audio::Track::TrackType::kMIDITrack)
    {
        return;
    }
    else
    {
        std::shared_ptr<Musec::Audio::Plugin::IPlugin<double>> plugin(nullptr);
        if(pluginFormat == Musec::Base::PluginFormat::FormatVST2)
        {
            plugin = std::static_pointer_cast<Musec::Audio::Plugin::IPlugin<double>>(
                std::make_shared<Musec::Audio::Plugin::VST2Plugin<double>>(path, false, pluginSubId)
            );
        }
        else if(pluginFormat == Musec::Base::PluginFormat::FormatVST3)
        {
            plugin = std::static_pointer_cast<Musec::Audio::Plugin::IPlugin<double>>(
                std::make_shared<Musec::Audio::Plugin::VST3Plugin<double>>(path, pluginSubId)
            );
        }
        if(trackType == Musec::Audio::Track::TrackType::kInstrumentTrack)
        {
            auto instrumentTrack = std::static_pointer_cast<Musec::Audio::Track::InstrumentTrack>(project_[trackIndex].track);
            auto pluginSequences = instrumentTrack->getAudioEffectPluginSequences();
            auto& pluginSequence = pluginSequences[0];
            pluginSequence.insert(pluginSequence.begin() + pluginIndex, plugin);
            instrumentTrack->setAudioEffectPluginSequences(std::move(pluginSequences));
        }
        else if(trackType == Musec::Audio::Track::TrackType::kAudioTrack)
        {
            auto audioTrack = std::static_pointer_cast<Musec::Audio::Track::AudioTrack>(project_[trackIndex].track);
            auto pluginSequences = audioTrack->getPluginSequences();
            auto& pluginSequence = pluginSequences[0];
            pluginSequence.insert(pluginSequence.begin() + pluginIndex, plugin);
            audioTrack->setPluginSequences(std::move(pluginSequences));
        }
    }
}

Musec::Model::RoleNamesType Musec::Model::TrackListModel::roleNames() const
{
    return roleNames_;
}

Qt::ItemFlags Musec::Model::TrackListModel::flags(const QModelIndex& index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemFlag::ItemIsEditable;
}
