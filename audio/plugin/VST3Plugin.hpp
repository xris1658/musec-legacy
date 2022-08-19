#ifndef MUSEC_AUDIO_PLUGIN_VST3PLUGIN
#define MUSEC_AUDIO_PLUGIN_VST3PLUGIN

#include "audio/plugin/IPlugin.hpp"
#include "audio/plugin/VST3PluginParameter.hpp"
#include "base/FixedSizeMemoryBlock.hpp"
#include "base/PluginBase.hpp"
#include "native/Native.hpp"
#include "native/WindowsLibraryRAII.hpp"

#include <pluginterfaces/gui/iplugview.h>
#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/vst/ivstmessage.h>
#include <pluginterfaces/vst/ivstnoteexpression.h>
#include <pluginterfaces/vst/ivstrepresentation.h>

#include <public.sdk/source/vst/hosting/connectionproxy.h>
#include <public.sdk/source/vst/hosting/parameterchanges.h>

#include <QString>
#include <QWindow>

#include <map>
#include <memory>

namespace Musec
{
namespace Audio
{
namespace Plugin
{
#if(WIN32)
constexpr char VST3PluginInitName[] = "InitDll";
constexpr char VST3PluginExitName[] = "ExitDll";
#elif(MAC)
constexpr char VST3PluginInitName[] = "bundleEntry";
constexpr char VST3PluginExitName[] = "bundleExit";
#elif(LINUX)
constexpr char VST3PluginInitName[] = "ModuleEntry";
constexpr char VST3PlugiNExitName[] = "ModuleExit";
#endif
// VST3 插件音频组件的状态。
// 参见：https://developer.steinberg.help/display/VST/Audio+Processor+Call+Sequence
enum class VST3AudioProcessorStatus: std::int8_t
{
    NoAudioProcessor = 0x00,
    Factory = 0x01,
    Created = 0x02,
    Initialized = 0x04,
    Connected = 0x08,
    SetupDone = 0x10,
    Activated = 0x20,
    Processing = 0x40
};

// VST3 插件控制器（用户界面）的状态。
// 参见：https://developer.steinberg.help/display/VST/Edit+Controller+Call+Sequence
enum class VST3EditControllerStatus: std::int8_t
{
    NoEditController = 0x00,
    Factory = 0x01,
    Created = 0x02,
    Initialized = 0x04,
    Connected = 0x08
};

enum class EffectAndEditorUnified: std::int8_t
{
    NotUnified = 0,
    Unified = 1
};

using SpeakerArrangements = std::vector<Steinberg::Vst::SpeakerArrangement>;

constexpr Steinberg::uint32 BasicProcessContextRequirement =
    Steinberg::Vst::IProcessContextRequirements::Flags::kNeedTransportState
    | Steinberg::Vst::IProcessContextRequirements::Flags::kNeedTempo
    | Steinberg::Vst::IProcessContextRequirements::Flags::kNeedSystemTime
    | Steinberg::Vst::IProcessContextRequirements::Flags::kNeedTimeSignature;

class VST3Plugin:
    public Musec::Native::WindowsLibraryRAII,
    public Musec::Audio::Plugin::IPlugin,
    public Steinberg::Vst::IComponentHandler,
    public Steinberg::Vst::IComponentHandler2,
    public Steinberg::IPlugFrame
{
    using SampleType = float;
    using Base = Musec::Native::WindowsLibraryRAII;
    using SampleTypePointers = std::vector<SampleType*>;
public:
public: // ctor & dtor
    VST3Plugin();
    VST3Plugin(const QString& path, int classIndex);
    ~VST3Plugin() override;
public:
    const Steinberg::PClassInfo& getClassInfo() const;
    Steinberg::IPluginFactory* factory() const;
    Steinberg::Vst::IComponent* component() const;
    Steinberg::Vst::IAudioProcessor* effect() const;
    Steinberg::Vst::IEditController* editController() const;
    Steinberg::IPlugView* getView() const;
public: // IDevice interfaces
    std::uint8_t inputCount() const override;
    std::uint8_t outputCount() const override;
    void process(Audio::Base::AudioBufferView<SampleType>* inputs, int inputCount,
        Audio::Base::AudioBufferView<SampleType>* outputs, int outputCount) override;
public: // IPlugin interfaces
    bool activate() override;
    bool deactivate() override;
    bool initialize(double sampleRate, std::int32_t sampleCount) override;
    bool uninitialize() override;
    int parameterCount() override;
    IParameter& parameter(int index) override;
public:
    bool attachToWindow(QWindow* window) override;
    bool detachWithWindow() override;
    QWindow* window() override;
    bool startProcessing() override;
    bool stopProcessing() override;
    bool getBypass() const override;
    QString getName() const override;
    bool hasUI() override;
    Musec::Base::PluginFormat pluginFormat() override;
public: // FUnknown interfaces
    Steinberg::tresult queryInterface(const Steinberg::TUID iid, void** obj) override;
    Steinberg::uint32 addRef() override;
    Steinberg::uint32 release() override;
public: // IComponentHandler interfaces
    Steinberg::tresult PLUGIN_API beginEdit(Steinberg::Vst::ParamID id) override;
    Steinberg::tresult PLUGIN_API performEdit(Steinberg::Vst::ParamID id, Steinberg::Vst::ParamValue valueNormalized) override;
    Steinberg::tresult PLUGIN_API endEdit(Steinberg::Vst::ParamID id) override;
    Steinberg::tresult PLUGIN_API restartComponent(Steinberg::int32 flags) override;
public: // IComponentHandler2 interfaces
    Steinberg::tresult PLUGIN_API setDirty(Steinberg::TBool state) override;
    Steinberg::tresult PLUGIN_API requestOpenEditor(Steinberg::FIDString name = Steinberg::Vst::ViewType::kEditor) override;
    Steinberg::tresult PLUGIN_API startGroupEdit();
    Steinberg::tresult PLUGIN_API finishGroupEdit();
public: // IPluginFrame interfaces
    Steinberg::tresult resizeView(Steinberg::IPlugView* view, Steinberg::ViewRect* newSize) override;
public:
    const SpeakerArrangements& inputSpeakerArrangements();
    const SpeakerArrangements& outputSpeakerArrangements();
    bool activated() override;
    bool processing() override;
private:
    bool initializeEditor();
    bool uninitializeEditor();
    void onWindowSizeChanged();
private:
    Steinberg::PClassInfo classInfo_;
    Steinberg::IPluginFactory* factory_ = nullptr;
    Steinberg::Vst::IComponent* component_ = nullptr;
    // IAudioProcessor 和扩展接口
    Steinberg::Vst::IAudioProcessor* audioProcessor_ = nullptr;
    Steinberg::Vst::IAudioPresentationLatency* audioPresentationLatency_ = nullptr;
    Steinberg::Vst::IProcessContextRequirements* processContextRequirements_ = nullptr;
    Steinberg::uint32 processContextRequirement_ = BasicProcessContextRequirement;
    // IEditController 和扩展接口
    Steinberg::Vst::IEditController* editController_ = nullptr;
    Steinberg::Vst::IEditController2* editController2_ = nullptr;
    Steinberg::Vst::IMidiMapping* midiMapping_ = nullptr;
    Steinberg::Vst::IEditControllerHostEditing* editControllerHostEditing_ = nullptr;
    Steinberg::Vst::INoteExpressionController* noteExpressionController_ = nullptr;
    Steinberg::Vst::IKeyswitchController* keyswitchController_ = nullptr;
    Steinberg::Vst::IXmlRepresentationController* xmlRepresentationController_ = nullptr;
    // 用于 IComponentHandler 的成员
    Steinberg::int32 paramCount_ = 0;
    Musec::Base::FixedSizeMemoryBlock paramBlock_ = Musec::Base::FixedSizeMemoryBlock();
    Steinberg::Vst::IConnectionPoint* componentPoint_ = nullptr;
    Steinberg::Vst::IConnectionPoint* editControllerPoint_ = nullptr;
    Steinberg::IPlugView* view_ = nullptr;
    int audioInputBusIndex_ = -1;
    int audioOutputBusIndex_ = -1;
    // 输入参数改变
    Steinberg::Vst::ParameterChanges inputParameterChanges_;
    // 输出参数改变
    Steinberg::Vst::ParameterChanges outputParameterChanges_;
    // IAudioProcessor::process 函数调用的实参
    Steinberg::Vst::ProcessData processData_;
    // 调用 process 函数时将 data 赋值给 processData_
    std::vector<Steinberg::Vst::AudioBusBuffers> inputs_;
    // 调用 process 函数时将 data 赋值给 processData_
    std::vector<Steinberg::Vst::AudioBusBuffers> outputs_;
    // 输入音频缓冲区的原始数组
    std::vector<std::vector<SampleType*>> inputRaws_;
    // 输出音频缓冲区的原始数组
    std::vector<std::vector<SampleType*>> outputRaws_;
    // 各个总线的输入的扬声器布局
    SpeakerArrangements inputSpeakerArrangements_;
    // 各个总线输出的扬声器布局
    SpeakerArrangements outputSpeakerArrangements_;
    VST3AudioProcessorStatus audioProcessorStatus_ = VST3AudioProcessorStatus::NoAudioProcessor;
    VST3EditControllerStatus editControllerStatus_ = VST3EditControllerStatus::NoEditController;
    EffectAndEditorUnified effectAndEditorUnified_ = EffectAndEditorUnified::NotUnified;
    QWindow* window_ = nullptr;
    // 参数 ID 和对 paramBlock_ 对应的索引
    // 有必要把 std::map 换成定容量 AVL 吗？
    std::map<Steinberg::Vst::ParamID, int> paramIdAndIndex_;
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST3PLUGIN
