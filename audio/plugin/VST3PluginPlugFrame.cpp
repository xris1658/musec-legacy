#include "audio/plugin/VST3PluginPlugFrame.hpp"

#include "audio/plugin/VST3Plugin.hpp"

namespace Musec::Audio::Plugin
{
VST3PluginPlugFrame::VST3PluginPlugFrame(Musec::Audio::Plugin::VST3Plugin* plugin):
    plugin_(plugin)
{}

Steinberg::tresult VST3PluginPlugFrame::queryInterface(const Steinberg::int8* iid, void** obj)
{
    QUERY_INTERFACE(iid, obj, Steinberg::FUnknown::iid, Steinberg::IPlugFrame)
    QUERY_INTERFACE(iid, obj, Steinberg::IPlugFrame::iid, Steinberg::IPlugFrame)
    return Steinberg::kNoInterface;
}

Steinberg::uint32 VST3PluginPlugFrame::addRef()
{
    return 1;
}

Steinberg::uint32 VST3PluginPlugFrame::release()
{
    return 1;
}

Steinberg::tresult VST3PluginPlugFrame::resizeView(Steinberg::IPlugView* view, Steinberg::ViewRect* newSize)
{
    if(auto window = plugin_->window())
    {
        Steinberg::ViewRect oldSize; view->getSize(&oldSize);
        window->setWidth(newSize->getWidth());
        window->setHeight(newSize->getHeight());
        view->onSize(newSize);
        Steinberg::ViewRect newSize2; view->getSize(&newSize2);
        return Steinberg::kResultOk;
    }
}
}