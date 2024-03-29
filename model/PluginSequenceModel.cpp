#include "PluginSequenceModel.hpp"

#include "audio/track/AudioTrack.hpp"
#include "audio/track/InstrumentTrack.hpp"
#include "controller/AudioEngineController.hpp"

namespace Musec::Model
{
PluginSequenceModel::PluginSequenceModel(int trackIndex, QObject* parent):
    QAbstractListModel(parent)
{
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
    audioTrack_ = &Musec::Controller::AudioEngineController::AppProject().masterTrackRef().masterTrack;
}

PluginSequenceModel::~PluginSequenceModel()
{
    clear();
}

int PluginSequenceModel::itemCount() const
{
    if(instrumentTrack_)
    {
        if(auto& seqs = instrumentTrack_->getAudioEffectPluginSequences(); seqs.size())
        {
            return seqs[0].size();
        }
    }
    else if(audioTrack_)
    {
        if (auto& seqs = audioTrack_->getPluginSequences(); seqs.size())
        {
            return seqs[0].size();
        }
    }
    return 0;
}

constexpr int PluginSequenceModel::columnSize() noexcept
{
    return RoleNames::RoleEnd - Qt::UserRole;
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
    const Musec::Audio::Track::PluginSequence* pluginSequence = nullptr;
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
    case RoleNames::ProcessingRole:
        return QVariant::fromValue((*pluginSequence)[row]->processing());
    case RoleNames::NameRole:
        return QVariant::fromValue(names_[row]);
    case RoleNames::SidechainExistRole:
        return QVariant::fromValue(false); // FIXME
    case RoleNames::SidechainEnabledRole:
        return QVariant::fromValue(false);
    case RoleNames::WindowVisibleRole:
    {
        return QVariant::fromValue(pluginWindowConnections_[row]->isWindowVisible());
        // auto& plugin = (*pluginSequence)[row];
        // if(plugin->hasUI())
        // {
        //     return QVariant::fromValue(plugin->window()->isVisible());
        // }
        // return QVariant::fromValue(false);
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
    const Musec::Audio::Track::PluginSequence* pluginSequence = nullptr;
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
        pluginWindowConnections_[row]->setWindowVisible(value.value<bool>());
        return true;
        // auto& plugin = (*pluginSequence)[row];
        // if(plugin->hasUI())
        // {
        //     plugin->window()->setVisible(value.value<bool>());
        //     dataChanged(index, index, { RoleNames::WindowVisibleRole });
        //     return true;
        // }
        // return false;
    }
    case RoleNames::ProcessingRole:
    {
        auto& plugin = (*pluginSequence)[row];
        plugin->setProcessing(value.value<bool>());
        dataChanged(index, index, { RoleNames::ProcessingRole });
        return true;
    }
    default:
        return false;
    }
}

void PluginSequenceModel::insert(std::shared_ptr<Musec::Audio::Plugin::IPlugin> plugin, const QString& name, int index)
{
    names_.insert(names_.begin() + index, name);
    pluginWindowConnections_.insert(pluginWindowConnections_.begin() + index,
        std::make_unique<Musec::Entities::Plugin>(Musec::Entities::Plugin::fromPlugin(plugin, name)));
    connections_.insert(connections_.begin() + index,
        QObject::connect(pluginWindowConnections_[index].get(), &Musec::Entities::Plugin::windowVisibleChanged,
            this, [this, pluginIndex = index]()
            {
                dataChanged(this->index(pluginIndex), this->index(pluginIndex),
                    { RoleNames::WindowVisibleRole }
                );
            }
        ));
}

void PluginSequenceModel::replace(std::shared_ptr<Musec::Audio::Plugin::IPlugin> plugin, const QString& name, int index)
{
    QObject::disconnect(connections_[index]);
    pluginWindowConnections_[index].reset();
    pluginWindowConnections_[index] = std::make_unique<Musec::Entities::Plugin>(
        Musec::Entities::Plugin::fromPlugin(plugin, name));
    connections_[index] =
    QObject::connect(pluginWindowConnections_[index].get(), &Musec::Entities::Plugin::windowVisibleChanged,
        this, [this, pluginIndex = index]()
        {
            dataChanged(this->index(pluginIndex), this->index(pluginIndex),
                { RoleNames::WindowVisibleRole }
            );
        }
    );
    names_[index] = name;
}

void PluginSequenceModel::remove(int index)
{
    connections_.erase(connections_.begin() + index);
    pluginWindowConnections_.erase(pluginWindowConnections_.begin() + index);
    names_.erase(names_.begin() + index);
}

#pragma warning(push)
// the usage of 'QObject::disconnect' requires the compiler to capture 'this' but the current
// default capture mode does not allow it
#pragma warning(disable: 4573)
void PluginSequenceModel::clear()
{
    std::for_each(connections_.begin(), connections_.end(), [](const QMetaObject::Connection& connection) { QObject::disconnect(connection); });
    connections_.clear();
    pluginWindowConnections_.clear();
    names_.clear();
}
#pragma warning(pop)

RoleNamesType PluginSequenceModel::roleNames() const
{
    static const RoleNamesType ret =
    {
        std::make_pair(PluginSequenceModel::RoleNames::ValidRole, "valid"),
        std::make_pair(PluginSequenceModel::RoleNames::ProcessingRole, "processing"),
        std::make_pair(PluginSequenceModel::RoleNames::NameRole, "plugin_name"),
        std::make_pair(PluginSequenceModel::RoleNames::SidechainExistRole, "sidechain_exist"),
        std::make_pair(PluginSequenceModel::RoleNames::SidechainEnabledRole, "sidechain_enabled"),
        std::make_pair(PluginSequenceModel::RoleNames::WindowVisibleRole, "window_visible")
    };
    return ret;
}
}
