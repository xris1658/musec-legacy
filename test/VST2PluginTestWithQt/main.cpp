#include "../common/PluginWindowThread.hpp"

#include <audio/plugin/VST2Plugin.hpp>

#include <QGuiApplication>

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    PluginWindowThread pluginWindowThread;
    auto& window = *pluginWindowThread.window();
    pluginWindowThread.start();
    Musec::Audio::Plugin::VST2Plugin vst2(
        "C:\\Program Files\\VstPlugins\\WaveShell1-VST 13.1_x64.dll", false, 1279677267);
    auto inputCount = vst2.inputCount();
    auto outputCount = vst2.outputCount();
    auto effect = vst2.effect();
    std::vector<VstPinProperties> pinProperties(inputCount + outputCount, VstPinProperties());
    for(int i = 0; i < inputCount; ++i)
    {
        effect->dispatcher(effect, AEffectXOpcodes::effGetInputProperties, i, 0, pinProperties.data() + i, 0);
    }
    for(int i = 0; i < outputCount; ++i)
    {
        effect->dispatcher(effect, AEffectXOpcodes::effGetOutputProperties, i, 0, pinProperties.data() + i + inputCount, 0);
    }
    vst2.initialize(44100, 1024);
    vst2.initializeEditor();
    vst2.attachToWindow(&window);
    window.showNormal();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    vst2.activate();
    vst2.startProcessing();
    a.exec();
    vst2.detachWithWindow();
    vst2.stopProcessing();
    vst2.deactivate();
    window.close();
    vst2.uninitializeEditor();
    vst2.uninitialize();
    return 0;
}
