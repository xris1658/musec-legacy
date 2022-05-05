#include "Plugin.hpp"

namespace Musec
{
namespace Entities
{
Plugin::Plugin(QObject* parent):
    QObject(parent),
    plugin_(nullptr),
    name_(),
    enabled_(false),
    sidechainExist_(false),
    sidechainEnabled_(false)
{
}

Plugin::Plugin(std::shared_ptr<Musec::Audio::Plugin::IPlugin<float>> plugin,
               const QString& name, bool enabled,
               bool sidechainExist, bool sidechainEnabled):
    QObject(nullptr),
    plugin_(plugin),
    name_(name),
    enabled_(enabled),
    sidechainExist_(sidechainExist),
    sidechainEnabled_(sidechainEnabled)
{
    if(plugin_ && plugin_->window())
    {
        QObject::connect(plugin_->window(), &QWindow::visibleChanged,
            this, [this](bool visible)
            {
                windowVisibleChanged();
            }
        );
    }
}

Plugin::Plugin(Plugin&& rhs) noexcept
{
    swap(rhs);
}

Plugin& Plugin::operator=(Plugin&& rhs) noexcept
{
    if(this != &rhs)
    {
        swap(rhs);
    }
    return *this;
}

bool Plugin::valid() const
{
    return plugin_.get();
}

const QString& Plugin::getName() const
{
    return name_;
}

void Plugin::setName(const QString& name)
{
    name_ = name;
}

bool Plugin::isEnabled() const
{
    return enabled_;
}

void Plugin::setEnabled(bool enabled)
{
    enabled_ = enabled;
}

bool Plugin::isSidechainExist() const
{
    return sidechainExist_;
}

void Plugin::setSidechainExist(bool sidechainExist)
{
    sidechainExist_ = sidechainExist;
}

bool Plugin::isSidechainEnabled() const
{
    return sidechainEnabled_;
}

void Plugin::setSidechainEnabled(bool sidechainEnabled)
{
    sidechainEnabled_ = sidechainEnabled;
}

void Plugin::swap(Plugin& rhs) noexcept
{
    std::swap(plugin_, rhs.plugin_);
    std::swap(name_, rhs.name_);
    std::swap(enabled_, rhs.enabled_);
    std::swap(sidechainExist_, rhs.sidechainExist_);
    std::swap(sidechainEnabled_, rhs.sidechainEnabled_);
    nameChanged();
    enabledChanged();
    sidechainExistChanged();
    sidechainEnabledChanged();
    rhs.nameChanged();
    rhs.enabledChanged();
    rhs.sidechainExistChanged();
    rhs.sidechainEnabledChanged();
}

bool Plugin::isWindowVisible() const
{
    return plugin_ && plugin_->window() && plugin_->window()->isVisible();
}

void Plugin::setWindowVisible(bool windowVisible)
{
    if(plugin_ && plugin_->window())
    {
        if(windowVisible)
        {
            plugin_->window()->show();
        }
        else
        {
            plugin_->window()->hide();
        }
    }
}
}
}

namespace std
{
template<> void swap(Musec::Entities::Plugin& lhs, Musec::Entities::Plugin& rhs) noexcept
{
    lhs.swap(rhs);
}
}
