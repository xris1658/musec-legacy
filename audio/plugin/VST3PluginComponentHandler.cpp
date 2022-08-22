#include "audio/plugin/VST3PluginComponentHandler.hpp"

#include "audio/plugin/VST3Plugin.hpp"

namespace Musec::Audio::Plugin
{
VST3PluginComponentHandler::VST3PluginComponentHandler(Musec::Audio::Plugin::VST3Plugin* plugin):
    plugin_(plugin),
    paramIdAndIndex_()
{}

Steinberg::tresult VST3PluginComponentHandler::queryInterface(const Steinberg::int8* iid, void** obj)
{
    QUERY_INTERFACE(iid, obj, Steinberg::FUnknown::iid, Steinberg::Vst::IComponentHandler)
    QUERY_INTERFACE(iid, obj, Steinberg::Vst::IComponentHandler::iid, Steinberg::Vst::IComponentHandler)
    QUERY_INTERFACE(iid, obj, Steinberg::Vst::IComponentHandler2::iid, Steinberg::Vst::IComponentHandler2)
    return Steinberg::kNoInterface;
}

Steinberg::uint32 VST3PluginComponentHandler::addRef()
{
    return 1;
}

Steinberg::uint32 VST3PluginComponentHandler::release()
{
    return 1;
}

Steinberg::tresult VST3PluginComponentHandler::beginEdit(Steinberg::Vst::ParamID id)
{
    if(auto paramCount = plugin_->parameterCount())
    {
        for(decltype(paramCount) i = 0; i < paramCount; ++i)
        {
            auto& parameter = static_cast<VST3PluginParameter&>(plugin_->parameter(i));
            if(id == parameter.getParameterInfo().id)
            {
                paramIdAndIndex_.insert({id, i});
                return Steinberg::kResultOk;
            }
        }
    }
    return Steinberg::kInvalidArgument;
}

Steinberg::tresult VST3PluginComponentHandler::performEdit(Steinberg::Vst::ParamID id,
    Steinberg::Vst::ParamValue valueNormalized)
{
    plugin_->editController()->setParamNormalized(id, valueNormalized);
    // TODO: 用 paramIdAndIndex_ 中对应的索引值对 UI 等进行更新
    auto index = paramIdAndIndex_[id];
    return Steinberg::kResultOk;
}

Steinberg::tresult VST3PluginComponentHandler::endEdit(Steinberg::Vst::ParamID id)
{
    auto index = paramIdAndIndex_[id];
    paramIdAndIndex_.erase(id);
    return Steinberg::kResultOk;
}

Steinberg::tresult VST3PluginComponentHandler::restartComponent(Steinberg::int32 flags)
{
    return Steinberg::kNotImplemented;
}

Steinberg::tresult VST3PluginComponentHandler::setDirty(Steinberg::TBool state)
{
    return Steinberg::kNotImplemented;
}

Steinberg::tresult VST3PluginComponentHandler::requestOpenEditor(Steinberg::FIDString name)
{
    if(auto window = plugin_->window())
    {
        window->showNormal();
        return Steinberg::kResultOk;
    }
    return Steinberg::kResultFalse;
}

Steinberg::tresult VST3PluginComponentHandler::startGroupEdit()
{
    return Steinberg::kNotImplemented;
}

Steinberg::tresult VST3PluginComponentHandler::finishGroupEdit()
{
    return Steinberg::kNotImplemented;
}
}