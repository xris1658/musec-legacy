#include "VST2PluginPool.hpp"

#include <algorithm>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
VST2PluginPool::VST2PluginPool()
{

}

VST2PluginPool::VST2PluginPool(VST2PluginPool&& rhs): plugins_(std::move(rhs.plugins_))
{

}

VST2PluginPool::~VST2PluginPool()
{
    clear();
}

void VST2PluginPool::addPlugin(AEffect* plugin)
{
    if(plugins_.size() == plugins_.capacity())
    {
        while(lock_) {}
        lock_ = true;
        plugins_.emplace_back(plugin);
        lock_ = false;
        return;
    }
    plugins_.emplace_back(plugin);
}

void VST2PluginPool::removePlugin(AEffect* plugin)
{
    while(lock_) {}
    lock_ = true;
    auto it = std::find(plugins_.begin(), plugins_.end(), plugin);
    if(it != plugins_.end())
    {
        std::swap(*it, plugins_.back());
        plugins_.erase(plugins_.end() - 1);
    }
    lock_ = false;
}

void VST2PluginPool::clear()
{
    plugins_.clear();
}

void VST2PluginPool::doIdle()
{
    while(lock_) {}
    lock_ = true;
    std::for_each(plugins_.begin(), plugins_.end(),
        [](AEffect* plugin)
        {
            plugin->dispatcher(plugin, AEffectOpcodes::effEditIdle, 0, 0, nullptr, 0);
        }
    );
    lock_ = false;
}
}
}
}