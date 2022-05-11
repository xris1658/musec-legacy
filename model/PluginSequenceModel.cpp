#include "PluginSequenceModel.hpp"

#include "audio/track/AudioTrack.hpp"
#include "audio/track/InstrumentTrack.hpp"
#include "controller/AudioEngineController.hpp"

namespace Musec
{
namespace Model
{
PluginSequenceModel::PluginSequenceModel(int trackIndex, QObject* parent):
    QAbstractListModel(parent)
{
    initRoleNames();
    auto track = Musec::Controller::AudioEngineController::AppProject()[trackIndex].track;
    if (track->trackType() == Musec::Audio::Track::TrackType::kInstrumentTrack)
    {
        instrumentTrack_ = std::static_pointer_cast<Musec::Audio::Track::InstrumentTrack>(track).get();
    }
    else if (track->trackType() == Musec::Audio::Track::TrackType::kAudioTrack)
    {
        audioTrack_ = std::static_pointer_cast<Musec::Audio::Track::AudioTrack>(track).get();
    }
}

PluginSequenceModel::PluginSequenceModel():
    QAbstractListModel(nullptr)
{
    initRoleNames();
    audioTrack_ = &Musec::Controller::AudioEngineController::AppProject().masterTrackRef().masterTrack;
}

PluginSequenceModel::~PluginSequenceModel()
{

}

void PluginSequenceModel::initRoleNames()
{
    roleNames_.reserve(columnSize());
    roleNames_[RoleNames::ValidRole] = "valid";
    roleNames_[RoleNames::ActivatedRole] = "activated";
    roleNames_[RoleNames::NameRole] = "plugin_name";
    roleNames_[RoleNames::SidechainExistRole] = "sidechain_exist";
    roleNames_[RoleNames::SidechainEnabledRole] = "sidechain_enabled";
    roleNames_[RoleNames::WindowVisibleRole] = "window_visible";
}

int PluginSequenceModel::itemCount() const
{
    if(instrumentTrack_)
    {
        return instrumentTrack_->getAudioEffectPluginSequences()[0].size();
    }
    else if(audioTrack_)
    {
        return audioTrack_->getPluginSequences()[0].size();
    }
    return 0;
}

constexpr int PluginSequenceModel::columnSize()
{
    return RoleNames::RoleNamesEnd - Qt::UserRole;
}

int PluginSequenceModel::rowCount(const QModelIndex&) const
{
    auto ret = itemCount();
    return ret;
}

int PluginSequenceModel::columnCount(const QModelIndex&) const
{
    return columnSize();
}

QVariant PluginSequenceModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= itemCount())
    {
        return QVariant();
    }
    const Musec::Audio::Track::PluginSequence<float>* pluginSequence = nullptr;
    if(instrumentTrack_)
    {
        pluginSequence = &(instrumentTrack_->getAudioEffectPluginSequences()[0]);
    }
    else if(audioTrack_)
    {
        pluginSequence = &(audioTrack_->getPluginSequences()[0]);
    }
    switch(role)
    {
    case RoleNames::ValidRole:
        return QVariant::fromValue(static_cast<bool>((*pluginSequence)[row]));
    case RoleNames::ActivatedRole:
        return QVariant::fromValue(false); // FIXME
    case RoleNames::NameRole:
        return QVariant::fromValue((*pluginSequence)[row]->getName());
    case RoleNames::SidechainExistRole:
        return QVariant::fromValue(false); // FIXME
    case RoleNames::SidechainEnabledRole:
        return QVariant::fromValue(false);
    case RoleNames::WindowVisibleRole:
    {
        auto& plugin = (*pluginSequence)[row];
        if(plugin->hasUI())
        {
            return QVariant::fromValue(plugin->window()->isVisible());
        }
        return QVariant::fromValue(false);
    }
    default:
        return QVariant();
    }
}

bool PluginSequenceModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    int row = index.row();
    if(row < 0 || row >= rowCount(index))
    {
        return false;
    }
    const Musec::Audio::Track::PluginSequence<float>* pluginSequence = nullptr;
    if(instrumentTrack_)
    {
        pluginSequence = &(instrumentTrack_->getAudioEffectPluginSequences()[0]);
    }
    else if(audioTrack_)
    {
        pluginSequence = &(audioTrack_->getPluginSequences()[0]);
    }
    switch(role)
    {
    case RoleNames::WindowVisibleRole:
    {
        auto& plugin = (*pluginSequence)[row];
        if(plugin->hasUI())
        {
            plugin->window()->setVisible(value.value<bool>());
            return true;
        }
        return false;
    }
    default:
        return false;
    }
}

void PluginSequenceModel::setWindowVisible(int effectIndex, bool visible)
{
    if(effectIndex < 0 || effectIndex >= itemCount())
    {
        return;
    }
    const Musec::Audio::Track::PluginSequence<float>* pluginSequence = nullptr;
    if(instrumentTrack_)
    {
        pluginSequence = &(instrumentTrack_->getAudioEffectPluginSequences()[0]);
    }
    else if(audioTrack_)
    {
        pluginSequence = &(audioTrack_->getPluginSequences()[0]);
    }
    auto plugin = (*pluginSequence)[effectIndex];
    auto window = plugin->window();
    if(plugin->hasUI() && window)
    {
        window->setVisible(visible);
        dataChanged(index(effectIndex), index(effectIndex), { RoleNames::WindowVisibleRole });
    }
}

RoleNamesType PluginSequenceModel::roleNames() const
{
    return roleNames_;
}
}
}
