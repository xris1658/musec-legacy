#include "TrackListModel.hpp"

#include "audio/track/InstrumentTrack.hpp"
#include "base/Base.hpp"
#include "controller/AudioEngineController.hpp"
#include "controller/PluginController.hpp"
#include "entities/Plugin.hpp"
#include "model/PluginSequenceModel.hpp"

#include <QColor>
#include <QVector>

#include <memory>

namespace Musec::Model
{
using namespace Musec::Entities;
namespace Impl
{
std::shared_ptr<Musec::Audio::Plugin::IPlugin> loadAndStartPlugin(const QString& path, int format, const char* id)
{
    auto plugin = Musec::Controller::PluginController::loadPlugin(path, format, id);
    if(plugin && Musec::Controller::PluginController::startPlugin(plugin))
    {
        return plugin;
    }
    return nullptr;
}
}

TrackListModel::TrackListModel(QObject* parent):
    QAbstractListModel(parent),
    project_(Musec::Controller::AudioEngineController::AppProject()),
    projectMasterTrackRef_(project_.masterTrackRef()),
    masterPluginSequences_()
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
    roleNames_[RoleNames::MonoDownMixRole] = "monoDownMix";
    roleNames_[RoleNames::InstrumentRole] = "instrument";
    roleNames_[RoleNames::PluginListRole] = "plugin_list";
    roleNames_[RoleNames::ChannelGainRole] = "channel_gain";
    roleNames_[RoleNames::ChannelPanningRole] = "channel_panning";
}

TrackListModel::~TrackListModel()
{
    project_.clear();
    masterPluginSequences_.clear();
    pluginSequences_.clear();
}

constexpr int TrackListModel::columnSize()
{
    return RoleNames::kRoleNamesCount - Qt::UserRole;
}

int TrackListModel::trackCount() const noexcept
{
    return project_.trackCount();
}

int TrackListModel::rowCount(const QModelIndex&) const
{
    return project_.trackCount();
}

int TrackListModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant TrackListModel::data(const QModelIndex& index, int role) const
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
        // TODO: Save the track type, instead of retrieving it from the track
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
    case RoleNames::MonoDownMixRole:
        return QVariant::fromValue(static_cast<bool>(track.trackMonoDownMix));
    case RoleNames::InstrumentRole:
    {
        if(track.track->trackType() != Musec::Audio::Track::kInstrumentTrack)
        {
            return QVariant();
        }
        // pointer to `const` can't be used in the meta-object system, so we need to remove `const`
        return QVariant::fromValue(const_cast<Plugin*>(instruments_[row].get()));
    }
    case RoleNames::PluginListRole:
    {
        const auto type = track.track->trackType();
        if(type == Musec::Audio::Track::kMIDITrack)
        {
            return QVariant();
        }
        else
        {
            return QVariant::fromValue(pluginSequences_[row].get());
        }
    }
    case RoleNames::ChannelGainRole:
        return QVariant(track.gain);
    case RoleNames::ChannelPanningRole:
        return QVariant(track.panning);
    default:
        return QVariant();
    }
}

bool TrackListModel::setData(const QModelIndex& index, const QVariant& value, int role)
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
        case RoleNames::MonoDownMixRole:
            trackRef.trackMonoDownMix = value.value<bool>();
            ret = true;
            break;
        case RoleNames::ChannelGainRole:
            trackRef.gain = value.value<double>();
            ret = true;
            break;
        case RoleNames::ChannelPanningRole:
            trackRef.panning = value.value<double>();
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

Musec::Model::PluginSequenceModel& TrackListModel::masterTrackPluginSequenceModel()
{
    return masterPluginSequences_;
}

void TrackListModel::insertTrack(int index, const CompleteTrack& track)
{
    emit beginInsertRows(QModelIndex(), index, index);
    project_.insertTrack(index, track);
    instruments_.insert(
        instruments_.begin() + index,
        std::make_unique<Plugin>(Plugin::fromPlugin(nullptr))
    );
    pluginSequences_.insert(
        pluginSequences_.begin() + index,
        std::make_unique<PluginSequenceModel>(index)
    );
    emit endInsertRows();
}

void TrackListModel::appendTrack(const CompleteTrack& track)
{
    insertTrack(trackCount(), track);
}

void TrackListModel::insertTrack(int index, const QJSValue& value)
{
    auto track = qobject_cast<CompleteTrack*>(value.toQObject());
    if(track)
    {
        insertTrack(index, *track);
    }
}

void TrackListModel::appendTrack(const QJSValue& value)
{
    auto track = qobject_cast<CompleteTrack*>(value.toQObject());
    if(track)
    {
        appendTrack(*track);
    }
}

void TrackListModel::removeTrack(int index)
{
    if(index < 0 || index >= trackCount())
    {
        return;
    }
    emit beginRemoveRows(QModelIndex(), index, index);
    instruments_.erase(instruments_.begin() + index);
    pluginSequences_.erase(pluginSequences_.begin() + index);
    project_.eraseTrack(index);
    emit endRemoveRows();
}

void TrackListModel::loadInstrument(int trackIndex, int pluginId)
{
    auto info = Musec::Controller::PluginController::pluginFromId(pluginId);
    const auto& pluginFormat = std::get<Musec::Base::PluginReadInfoField::ReadFieldFormat>(info);
    const auto& path = std::get<Musec::Base::PluginReadInfoField::ReadFieldPath>(info);
    const auto& pluginSubId = std::get<Musec::Base::PluginReadInfoField::ReadFieldUid>(info);
    const auto& name = std::get<Musec::Base::PluginReadInfoField::ReadFieldName>(info);
    auto track = project_[trackIndex];
    if(track.track->trackType() != Musec::Audio::Track::TrackType::kInstrumentTrack)
    {
        return;
    }
    auto instrumentTrack = std::static_pointer_cast<Musec::Audio::Track::InstrumentTrack>(project_[trackIndex].track);
    auto instrument = Impl::loadAndStartPlugin(path, pluginFormat, pluginSubId.data());
    if(instrument)
    {
        instrumentTrack->setInstrument(instrument);
        instruments_[trackIndex] = std::make_unique<Plugin>(Plugin::fromPlugin(instrument, name));
        dataChanged(index(trackIndex), index(trackIndex), QVector<int>(1, RoleNames::InstrumentRole));
    }
}

void TrackListModel::insertEffect(int trackIndex, int pluginIndex, int pluginId)
{
    auto info = Musec::Controller::PluginController::pluginFromId(pluginId);
    const auto& pluginFormat = std::get<Musec::Base::PluginReadInfoField::ReadFieldFormat>(info);
    const auto& path = std::get<Musec::Base::PluginReadInfoField::ReadFieldPath>(info);
    const auto& pluginSubId = std::get<Musec::Base::PluginReadInfoField::ReadFieldUid>(info);
    const auto& name = std::get<Musec::Base::PluginReadInfoField::ReadFieldName>(info);
    auto track = project_[trackIndex];
    auto trackType = track.track->trackType();
    if(trackType == Musec::Audio::Track::TrackType::kMIDITrack)
    {
        return;
    }
    auto plugin = Impl::loadAndStartPlugin(path, pluginFormat, pluginSubId.data());
    if(plugin)
    {
        auto& pluginSequenceModel = pluginSequences_[trackIndex];
        if(trackType == Musec::Audio::Track::TrackType::kInstrumentTrack)
        {
            auto instrumentTrack = std::static_pointer_cast<Musec::Audio::Track::InstrumentTrack>(project_[trackIndex].track);
            pluginSequenceModel->beginInsertRows(QModelIndex(), pluginIndex, pluginIndex);
            auto pluginSequences = instrumentTrack->getAudioEffectPluginSequences();
            pluginSequences[0].insert(pluginSequences[0].begin() + pluginIndex, plugin);
            instrumentTrack->setAudioEffectPluginSequences(std::move(pluginSequences));
            pluginSequenceModel->endInsertRows();
        }
        else if(trackType == Musec::Audio::Track::TrackType::kAudioTrack)
        {
            auto audioTrack = std::static_pointer_cast<Musec::Audio::Track::AudioTrack>(project_[trackIndex].track);
            pluginSequenceModel->beginInsertRows(QModelIndex(), pluginIndex, pluginIndex);
            auto pluginSequences = audioTrack->getPluginSequences();
            auto& pluginSequence = pluginSequences[0];
            pluginSequence.insert(pluginSequence.begin() + pluginIndex, plugin);
            audioTrack->setPluginSequences(std::move(pluginSequences));
            pluginSequenceModel->endInsertRows();
        }
        pluginSequenceModel->insert(plugin, name, pluginIndex);
        dataChanged(index(trackIndex), index(trackIndex), QVector<int>(1, RoleNames::PluginListRole));
    }
}

void TrackListModel::replaceEffect(int trackIndex, int pluginIndex, int pluginId)
{
    auto info = Musec::Controller::PluginController::pluginFromId(pluginId);
    const auto& pluginFormat = std::get<Musec::Base::PluginReadInfoField::ReadFieldFormat>(info);
    const auto& path = std::get<Musec::Base::PluginReadInfoField::ReadFieldPath>(info);
    const auto& pluginSubId = std::get<Musec::Base::PluginReadInfoField::ReadFieldUid>(info);
    const auto& name = std::get<Musec::Base::PluginReadInfoField::ReadFieldName>(info);
    auto track = project_[trackIndex];
    auto trackType = track.track->trackType();
    if(trackType == Musec::Audio::Track::TrackType::kMIDITrack)
    {
        return;
    }
    auto plugin = Impl::loadAndStartPlugin(path, pluginFormat, pluginSubId.data());
    if(plugin)
    {
        auto& pluginSequenceModel = pluginSequences_[trackIndex];
        if (trackType == Musec::Audio::Track::TrackType::kInstrumentTrack)
        {
            auto instrumentTrack = std::static_pointer_cast<Musec::Audio::Track::InstrumentTrack>(
                project_[trackIndex].track
            );
            auto pluginSequences = instrumentTrack->getAudioEffectPluginSequences();
            pluginSequences[0][pluginIndex].reset();
            pluginSequences[0][pluginIndex] = std::move(plugin);
            instrumentTrack->setAudioEffectPluginSequences(std::move(pluginSequences));
        }
        else if (trackType == Musec::Audio::Track::TrackType::kAudioTrack)
        {
            auto audioTrack = std::static_pointer_cast<Musec::Audio::Track::AudioTrack>(project_[trackIndex].track);
            auto pluginSequences = audioTrack->getPluginSequences();
            pluginSequences[0][pluginIndex].reset();
            pluginSequences[0][pluginIndex] = plugin;
            audioTrack->setPluginSequences(std::move(pluginSequences));
        }
        pluginSequenceModel->replace(plugin, name, pluginIndex);
        pluginSequenceModel->dataChanged(index(pluginIndex), index(pluginIndex), {});
        dataChanged(index(trackIndex), index(trackIndex), QVector<int>(1, RoleNames::PluginListRole));
    }
}

void TrackListModel::insertEffectMasterTrack(int pluginIndex, int pluginId)
{
    auto info = Musec::Controller::PluginController::pluginFromId(pluginId);
    const auto& pluginFormat = std::get<Musec::Base::PluginReadInfoField::ReadFieldFormat>(info);
    const auto& path = std::get<Musec::Base::PluginReadInfoField::ReadFieldPath>(info);
    const auto& pluginSubId = std::get<Musec::Base::PluginReadInfoField::ReadFieldUid>(info);
    const auto& name = std::get<Musec::Base::PluginReadInfoField::ReadFieldName>(info);
    auto& track = projectMasterTrackRef_.masterTrack;
    auto plugin = Impl::loadAndStartPlugin(path, pluginFormat, pluginSubId.data());
    if(plugin)
    {
        auto& masterTrackPluginSequenceModel = this->masterTrackPluginSequenceModel();
        masterTrackPluginSequenceModel.beginInsertRows(QModelIndex(), pluginIndex, pluginIndex);
        auto masterTrackPluginSequences = track.getPluginSequences();
        masterTrackPluginSequences[0].insert(masterTrackPluginSequences[0].begin() + pluginIndex, plugin);
        track.setPluginSequences(masterTrackPluginSequences);
        masterTrackPluginSequenceModel.endInsertRows();
        masterTrackPluginSequenceModel.insert(plugin, name, pluginIndex);
    }
}

void TrackListModel::replaceEffectMasterTrack(int pluginIndex, int pluginId)
{
    auto info = Musec::Controller::PluginController::pluginFromId(pluginId);
    const auto& pluginFormat = std::get<Musec::Base::PluginReadInfoField::ReadFieldFormat>(info);
    const auto& path = std::get<Musec::Base::PluginReadInfoField::ReadFieldPath>(info);
    const auto& pluginSubId = std::get<Musec::Base::PluginReadInfoField::ReadFieldUid>(info);
    const auto& name = std::get<Musec::Base::PluginReadInfoField::ReadFieldName>(info);
    auto& track = projectMasterTrackRef_.masterTrack;
    auto plugin = Impl::loadAndStartPlugin(path, pluginFormat, pluginSubId.data());
    if(plugin)
    {
        auto& masterTrackPluginSequenceModel = this->masterTrackPluginSequenceModel();
        masterTrackPluginSequenceModel.beginInsertRows(QModelIndex(), pluginIndex, pluginIndex);
        auto masterTrackPluginSequences = track.getPluginSequences();
        masterTrackPluginSequences[0][pluginIndex].reset();
        masterTrackPluginSequences[0][pluginIndex] = plugin;
        track.setPluginSequences(std::move(masterTrackPluginSequences));
        masterTrackPluginSequenceModel.replace(plugin, name, pluginIndex);
        masterTrackPluginSequenceModel.dataChanged(index(pluginIndex), index(pluginIndex), {});
    }
}

void TrackListModel::removeInstrument(int trackIndex)
{
    if(trackIndex < 0 || trackIndex >= trackCount())
    {
        return;
    }
    auto track = project_[trackIndex].track;
    if(track->trackType() == Musec::Audio::Track::kInstrumentTrack)
    {
        auto instrumentTrack = std::static_pointer_cast<Musec::Audio::Track::InstrumentTrack>(track);
        instrumentTrack->setInstrument(nullptr);
        *(instruments_[trackIndex]) = Plugin::fromPlugin();
        dataChanged(this->index(trackIndex), this->index(trackIndex), { RoleNames::InstrumentRole });
    }
}

void TrackListModel::removeEffect(int trackIndex, int pluginIndex)
{
    if(trackIndex < 0 || trackIndex >= trackCount())
    {
        return;
    }
    auto track = project_[trackIndex].track;
    auto& pluginSequenceModel = pluginSequences_[trackIndex];
    if(track->trackType() == Musec::Audio::Track::kInstrumentTrack)
    {
        auto instrumentTrack = std::static_pointer_cast<Musec::Audio::Track::InstrumentTrack>(track);
        pluginSequenceModel->beginRemoveRows(QModelIndex(), pluginIndex, pluginIndex);
        auto pluginSequences = instrumentTrack->getAudioEffectPluginSequences();
        pluginSequences[0].erase(pluginSequences[0].begin() + pluginIndex);
        instrumentTrack->setAudioEffectPluginSequences(std::move(pluginSequences));
        pluginSequenceModel->endRemoveRows();
    }
    else if(track->trackType() == Musec::Audio::Track::kAudioTrack)
    {
        auto audioTrack = std::static_pointer_cast<Musec::Audio::Track::AudioTrack>(track);
        pluginSequenceModel->beginRemoveRows(QModelIndex(), pluginIndex, pluginIndex);
        auto pluginSequences = audioTrack->getPluginSequences();
        pluginSequences[0].erase(pluginSequences[0].begin() + pluginIndex);
        audioTrack->setPluginSequences(std::move(pluginSequences));
        pluginSequenceModel->endRemoveRows();
    }
    pluginSequenceModel->remove(pluginIndex);
}

void TrackListModel::copyInstrument(int fromTrackIndex, int toTrackIndex)
{

}

void TrackListModel::copyEffect(
    int fromTrackIndex, int fromPluginIndex, int toTrackIndex, int toPluginIndex)
{

}

void TrackListModel::moveInstrument(int fromTrackIndex, int toTrackIndex)
{

}

void TrackListModel::moveEffect(
    int fromTrackIndex, int fromPluginIndex, int toTrackIndex, int toPluginIndex)
{

}

void TrackListModel::removeEffectMasterTrack(int pluginIndex)
{
    auto& track = projectMasterTrackRef_.masterTrack;
    auto& masterTrackPluginSequenceModel = this->masterTrackPluginSequenceModel();
    masterTrackPluginSequenceModel.beginRemoveRows(QModelIndex(), pluginIndex, pluginIndex);
    auto masterTrackPluginSequences = track.getPluginSequences();
    masterTrackPluginSequences[0].erase(masterTrackPluginSequences[0].begin() + pluginIndex);
    track.setPluginSequences(std::move(masterTrackPluginSequences));
    masterTrackPluginSequenceModel.remove(pluginIndex);
    masterTrackPluginSequenceModel.endRemoveRows();
}

RoleNamesType TrackListModel::roleNames() const
{
    return roleNames_;
}

Qt::ItemFlags TrackListModel::flags(const QModelIndex& index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemFlag::ItemIsEditable;
}

double TrackListModel::getMasterTrackGain() const
{
    return project_.masterTrackRef().gain;
}

void TrackListModel::setMasterTrackGain(double gain)
{
    projectMasterTrackRef_.gain = gain;
    masterTrackGainChanged();
}

double TrackListModel::getMasterTrackPanning() const
{
    return project_.masterTrackRef().panning;
}

void TrackListModel::setMasterTrackPanning(double panning)
{
    projectMasterTrackRef_.panning = panning;
    masterTrackPanningChanged();
}

bool TrackListModel::getMasterTrackMute() const
{
    return projectMasterTrackRef_.trackMute;
}

void TrackListModel::setMasterTrackMute(bool mute)
{
    projectMasterTrackRef_.trackMute = mute;
    masterTrackMuteChanged();
}

bool TrackListModel::getMasterTrackSolo() const
{
    return projectMasterTrackRef_.trackSolo;
}

void TrackListModel::setMasterTrackSolo(bool solo)
{
    projectMasterTrackRef_.trackSolo = solo;
    masterTrackSoloChanged();
}

bool TrackListModel::getMasterTrackInvertPhase() const
{
    return projectMasterTrackRef_.trackInvertPhase;
}

void TrackListModel::setMasterTrackInvertPhase(bool invertPhase)
{
    projectMasterTrackRef_.trackInvertPhase = invertPhase;
    masterTrackInvertPhaseChanged();
}

bool TrackListModel::getMasterTrackArmRecording() const
{
    return projectMasterTrackRef_.trackArmRecording;
}

void TrackListModel::setMasterTrackArmRecording(bool armRecording)
{
    projectMasterTrackRef_.trackArmRecording = armRecording;
    masterTrackArmRecordingChanged();
}

bool TrackListModel::getMasterTrackMonoDownMix() const
{
    return projectMasterTrackRef_.trackMonoDownMix;
}

void TrackListModel::setMasterTrackMonoDownMix(bool monoDownMix)
{
    projectMasterTrackRef_.trackMonoDownMix  = monoDownMix;
    masterTrackMonoDownMixChanged();
}
}
