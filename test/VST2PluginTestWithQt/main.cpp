#include "../common/PluginWindowThread.hpp"

#include <audio/plugin/VST2Plugin.hpp>

#include <QGuiApplication>

#include <thread>
#include <chrono>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    PluginWindowThread pluginWindowThread;
    auto& window = *pluginWindowThread.window();
    pluginWindowThread.start();
    Musec::Audio::Plugin::VST2Plugin<double> vst2(
        "C:\\Program Files\\VstPlugins\\WaveShell1-VST 13.1_x64.dll", false, 1279677267);
    vst2.initialize(44100, 1024);
    vst2.initializeEditor(&window);
    window.showNormal();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    vst2.activate();
    vst2.startProcessing();
    a.exec();
    vst2.stopProcessing();
    vst2.deactivate();
    window.close();
    vst2.uninitializeEditor();
    vst2.uninitialize();
    return 0;
}
