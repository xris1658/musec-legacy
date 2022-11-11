#ifndef MUSEC_AUDIO_PLUGIN_VST3PLUGIN
#define MUSEC_AUDIO_PLUGIN_VST3PLUGIN

#include "audio/plugin/IPlugin.hpp"
#include "audio/plugin/VST3PluginParameter.hpp"
#include "audio/plugin/VST3PluginComponentHandler.hpp"
#include "audio/plugin/VST3PluginPlugFrame.hpp"
#include "audio/plugin/VST3SpeakerGroupCollection.hpp"
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

#include <QMetaObject>
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
#elif
#error Unabled to determine init and exit name
#endif

// Reference: https://developer.steinberg.help/display/VST/Audio+Processor+Call+Sequence
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

// Reference: https://developer.steinberg.help/display/VST/Edit+Controller+Call+Sequence
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
    public Musec::Audio::Plugin::IPlugin
{
    using SampleType = float;
    using Base = Musec::Native::WindowsLibraryRAII;
    using SampleTypePointers = std::vector<SampleType*>;
public: // ctor & dtor
    VST3Plugin();
    VST3Plugin(const QString& path);
    bool createPlugin(const Steinberg::PClassInfo& classInfo);
    bool createPlugin(int classIndex);
    VST3Plugin(const QString& path, int classIndex);
    VST3Plugin(const QString& path, const Steinberg::PClassInfo& classInfo);
    ~VST3Plugin() override;
public:
    const Steinberg::PClassInfo& getClassInfo() const;
    Steinberg::IPluginFactory* factory() const;
    Steinberg::Vst::IComponent* component() const;
    Steinberg::Vst::IAudioProcessor* effect() const;
    Steinberg::Vst::IEditController* editController() const;
    Steinberg::IPlugView* getView() const;
public: // IDevice interfaces
    std::uint8_t audioInputCount() const override;
    std::uint8_t audioOutputCount() const override;
    const SpeakerArrangements& inputSpeakerArrangements();
    const SpeakerArrangements& outputSpeakerArrangements();
    const ISpeakerGroupCollection& audioInputSpeakerGroupCollection() const override;
    const ISpeakerGroupCollection& audioOutputSpeakerGroupCollection() const override;
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
public:
    bool activated() override;
    bool processing() override;
private:
    bool initializeEditController();
    bool uninitializeEditController();
    void initializeWindowSizeConnections();
    void uninitializeWindowSizeConnections();
    void onWindowSizeChanged();
public:
    Steinberg::Vst::ProcessSetup processSetup();
private:
    Steinberg::PClassInfo classInfo_;
    Steinberg::IPluginFactory* factory_ = nullptr;
    Steinberg::Vst::IComponent* component_ = nullptr;
    // IAudioProcessor and extension
    Steinberg::Vst::IAudioProcessor* audioProcessor_ = nullptr;
    Steinberg::Vst::IAudioPresentationLatency* audioPresentationLatency_ = nullptr;
    Steinberg::Vst::IProcessContextRequirements* processContextRequirements_ = nullptr;
    Steinberg::uint32 processContextRequirement_ = BasicProcessContextRequirement;
    // IEditController and extension
    Steinberg::Vst::IEditController* editController_ = nullptr;
    Steinberg::Vst::IEditController2* editController2_ = nullptr;
    Steinberg::Vst::IMidiMapping* midiMapping_ = nullptr;
    Steinberg::Vst::IEditControllerHostEditing* editControllerHostEditing_ = nullptr;
    Steinberg::Vst::INoteExpressionController* noteExpressionController_ = nullptr;
    Steinberg::Vst::IKeyswitchController* keyswitchController_ = nullptr;
    Steinberg::Vst::IXmlRepresentationController* xmlRepresentationController_ = nullptr;
    Steinberg::int32 paramCount_ = 0;
    Musec::Base::FixedSizeMemoryBlock paramBlock_ = Musec::Base::FixedSizeMemoryBlock();
    Steinberg::Vst::IConnectionPoint* componentPoint_ = nullptr;
    Steinberg::Vst::IConnectionPoint* editControllerPoint_ = nullptr;
    Steinberg::IPlugView* view_ = nullptr;
    int audioInputBusIndex_ = -1;
    int audioOutputBusIndex_ = -1;
    Steinberg::Vst::ParameterChanges inputParameterChanges_;
    Steinberg::Vst::ParameterChanges outputParameterChanges_;
    // Used for calling `IAudioProcessor::process`
    Steinberg::Vst::ProcessData processData_;
    // assign this->data() to processData_ while calling `process`
    std::vector<Steinberg::Vst::AudioBusBuffers> inputs_;
    // ditto
    std::vector<Steinberg::Vst::AudioBusBuffers> outputs_;
    std::vector<std::vector<SampleType*>> inputRaws_;
    std::vector<std::vector<SampleType*>> outputRaws_;
    SpeakerArrangements inputSpeakerArrangements_;
    SpeakerArrangements outputSpeakerArrangements_;
    Musec::Audio::Plugin::VST3SpeakerGroupCollection inputSpeakerGroupCollection_;
    Musec::Audio::Plugin::VST3SpeakerGroupCollection outputSpeakerGroupCollection_;
    VST3AudioProcessorStatus audioProcessorStatus_ = VST3AudioProcessorStatus::NoAudioProcessor;
    VST3EditControllerStatus editControllerStatus_ = VST3EditControllerStatus::NoEditController;
    EffectAndEditorUnified effectAndEditorUnified_ = EffectAndEditorUnified::NotUnified;
    QWindow* window_ = nullptr;
    Musec::Audio::Plugin::VST3PluginComponentHandler componentHandler_;
    Musec::Audio::Plugin::VST3PluginPlugFrame plugFrame_;
    std::array<QMetaObject::Connection, 2> windowSizeConnections_;
    Steinberg::Vst::ProcessSetup processSetup_ = {};
};
}
}
}

#endif //MUSEC_AUDIO_PLUGIN_VST3PLUGIN
