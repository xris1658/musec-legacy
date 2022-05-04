#include "../common/PluginWindowThread.hpp"

#include <audio/plugin/VST3Plugin.hpp>

#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include <pluginterfaces/vst/ivstcomponent.h>

#include <QGuiApplication>
#include <QThread>
#include <QWindow>

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

int main(int argc, char** argv) try
{
	QGuiApplication application(argc, argv);
	PluginWindowThread pluginWindowThread;
	QWindow& window = *pluginWindowThread.window();
	pluginWindowThread.start();
	{
		Musec::Audio::Plugin::VST3Plugin<double> vst3(
			"C:\\Program Files\\Common Files\\VST3\\FabFilter\\FabFilter Pro-Q 3.vst3"
			, 0);
		try
		{
			vst3.initialize(44100, 1024);
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
		std::cout << " Input count: " << vst3.component()->getBusCount(Steinberg::Vst::MediaTypes::kAudio, Steinberg::Vst::BusDirections::kInput) << std::endl;
		const auto& inputSpeakerArrangements = vst3.inputSpeakerArrangements();
		for (const auto& arr : inputSpeakerArrangements)
		{
			std::cout << Steinberg::Vst::SpeakerArr::getSpeakerArrangementString(arr, false) << " (" << Steinberg::Vst::SpeakerArr::getChannelCount(arr) << " channels)" << std::endl;
		}
		std::cout << "Output count: " << vst3.component()->getBusCount(Steinberg::Vst::MediaTypes::kAudio, Steinberg::Vst::BusDirections::kOutput) << std::endl;
		const auto& outputSpeakerArangements = vst3.outputSpeakerArrangements();
		for (const auto& arr: outputSpeakerArangements)
		{
			std::cout << Steinberg::Vst::SpeakerArr::getSpeakerArrangementString(arr, false) << " (" << Steinberg::Vst::SpeakerArr::getChannelCount(arr) << " channels)" << std::endl;
		}
		vst3.initializeEditor();
		vst3.attachToWindow(&window)
		window.showNormal();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		vst3.activate();
		vst3.startProcessing();
		application.exec();
		vst3.detachWithWindow();
		vst3.stopProcessing();
		vst3.deactivate();
		window.close();
		vst3.uninitializeEditor();
		vst3.uninitialize();
	}
	return 0;
}
catch (Musec::Native::ErrorCodeType errorCode)
{
	std::cerr << errorCode << '\n';
}