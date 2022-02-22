#ifndef MUSEC_CONTROLLER_APPCONTROLLER
#define MUSEC_CONTROLLER_APPCONTROLLER

#include "model/PluginListModel.hpp"
#include "model/AssetDirectoryListModel.hpp"
#include "model/ASIODriverListModel.hpp"
#include "event/SplashScreen.hpp"

#include <yaml-cpp/yaml.h>

#include <QString>

#include <array>

namespace Musec
{
namespace Controller
{
// 初始化应用程序（打开启动屏后 -> 打开主界面前）
void initApplication(Musec::Event::SplashScreen* splashScreen);

// 寻找应用数据
bool findAppData();

// 初始化应用数据（建立数据库等）
void initAppData();

// 加载应用数据（读取数据库内容等）
void loadAppData();

// 插件列表
Musec::Model::PluginListModel& AppPluginList();

// MIDI 效果器列表
Musec::Model::PluginListModel& AppMidiEffectList();

// 乐器列表
Musec::Model::PluginListModel& AppInstrumentList();

// 音频效果器列表
Musec::Model::PluginListModel& AppAudioEffectList();

// ASIO 驱动列表
Musec::Model::ASIODriverListModel& AppASIODriverList();

// 素材目录列表
Musec::Model::AssetDirectoryListModel& AppAssetDirectoryList();

// 刷新插件列表
void refreshPluginList(bool rescan);

// 加载素材目录列表
void loadAssetDirectoryList();
}
}

#endif // MUSEC_CONTROLLER_APPCONTROLLER
