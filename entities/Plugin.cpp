#include "Plugin.hpp"

#include "ui/PluginWindow.hpp"

#include <utility>

namespace Musec
{
namespace Entities
{
Plugin::Plugin(QObject* parent):
    QObject(parent),
    plugin_(nullptr),
    name_(),
    sidechainExist_(false),
    sidechainEnabled_(false)
{
}

Plugin::Plugin(std::shared_ptr<Musec::Audio::Plugin::IPlugin> plugin, const QString& name, bool sidechainExist,
    bool sidechainEnabled):
    QObject(nullptr),
    plugin_(std::move(plugin)),
    name_(name),
    sidechainExist_(sidechainExist),
    sidechainEnabled_(sidechainEnabled)
{
    // TODO: If the plugin doesn't have GUI, create a basic plugin editor for it
    if(plugin_)
    {
        if(!plugin_->window())
        {
            Musec::UI::createBasicPluginEditor(this);
        }
    }
    initPluginWindow();
}

Plugin::Plugin(Plugin&& rhs) noexcept
{
    swap(rhs);
    initPluginWindow();
    setBasicPluginEditor(basicPluginEditor_);
}

Plugin& Plugin::operator=(Plugin&& rhs) noexcept
{
    if(this != &rhs)
    {
        swap(rhs);
        initPluginWindow();
        setBasicPluginEditor(basicPluginEditor_);
    }
    return *this;
}

Plugin::~Plugin()
{
    if(basicPluginEditor_)
    {
        QObject::disconnect(connectionWithBasicPluginEditor_);
        Musec::UI::destroyBasicPluginEditor(basicPluginEditor_);
    }
}

Musec::Entities::Plugin Plugin::fromPlugin(const std::shared_ptr<Musec::Audio::Plugin::IPlugin>& plugin, const QString& name)
{
    if(plugin)
    {
        return {plugin, name, false, false};
    }
    else
    {
        return {};
    }
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
    nameChanged();
}

bool Plugin::isProcessing() const
{
    return plugin_ && (plugin_->processing());

}

void Plugin::setProcessing(bool processing)
{
    if(plugin_)
    {
        plugin_->setProcessing(processing);
    }
    processingChanged();
}

bool Plugin::isSidechainExist() const
{
    return sidechainExist_;
}

void Plugin::setSidechainExist(bool sidechainExist)
{
    sidechainExist_ = sidechainExist;
    sidechainExistChanged();
}

bool Plugin::isSidechainEnabled() const
{
    return sidechainEnabled_;
}

void Plugin::setSidechainEnabled(bool sidechainEnabled)
{
    sidechainEnabled_ = sidechainEnabled;
    sidechainEnabledChanged();
}

void Plugin::swap(Plugin& rhs) noexcept
{
    std::swap(plugin_, rhs.plugin_);
    std::swap(basicPluginEditor_, rhs.basicPluginEditor_);
    std::swap(name_, rhs.name_);
    std::swap(sidechainExist_, rhs.sidechainExist_);
    std::swap(sidechainEnabled_, rhs.sidechainEnabled_);
    nameChanged();
    processingChanged();
    sidechainExistChanged();
    sidechainEnabledChanged();
    rhs.nameChanged();
    rhs.processingChanged();
    rhs.sidechainExistChanged();
    rhs.sidechainEnabledChanged();
}

bool Plugin::isWindowVisible() const
{
    if(plugin_ && plugin_->window())
    {
        return plugin_->window()->isVisible();
    }
    return basicPluginEditor_ && basicPluginEditor_->isVisible();
}

void Plugin::setWindowVisible(bool windowVisible)
{
    if(plugin_ && plugin_->window())
    {
        plugin_->window()->setVisible(windowVisible);
        windowVisibleChanged();
    }
    if(basicPluginEditor_)
    {
        basicPluginEditor_->setVisible(windowVisible);
        windowVisibleChanged();
    }
}

std::shared_ptr<Musec::Audio::Plugin::IPlugin> Plugin::plugin()
{
    return plugin_;
}

void Plugin::setBasicPluginEditor(QWindow* basicPluginEditor)
{
    basicPluginEditor_ = basicPluginEditor;
    if(basicPluginEditor_)
    {
        connectionWithBasicPluginEditor_ = QObject::connect(basicPluginEditor_, &QWindow::visibleChanged,
            this, [this](bool visible)
            {
                windowVisibleChanged();
            }
        );
    }
}

void Plugin::initPluginWindow()
{
    if(plugin_ && plugin_->window())
    {
        plugin_->window()->setTitle(name_);
        QObject::connect(plugin_->window(), &QWindow::visibleChanged,
            this, [this](bool visible)
            {
                windowVisibleChanged();
            }
        );
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
