#ifndef MUSEC_CONTROLLER_APPCONTROLLER
#define MUSEC_CONTROLLER_APPCONTROLLER

#include "dao/LoggingDAO.hpp"
#include "event/SplashScreen.hpp"
#include "model/ASIODriverListModel.hpp"
#include "model/AssetDirectoryListModel.hpp"
#include "model/PluginListModel.hpp"
#include "model/TranslationFileModel.hpp"

#include <QString>

#include <array>
#include <future>

namespace Musec
{
namespace Controller
{
// 创建日志文件
Musec::DAO::LoggingDAO::LoggerType& AppLogger();

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

// 翻译文件列表
Musec::Model::TranslationFileModel& AppTranslationFileList();

// 刷新插件列表
void refreshPluginList(bool rescan);

// 加载素材目录列表
void loadAssetDirectoryList();

// 打开用于输入特殊字符的程序
void openSpecialCharacterInput();

// 用于 SplashScreenWorkThread 和主线程交流的 promise（主线程收）
std::promise<QString>& loadTranslationPromiseStart();

// 用于 SplashScreenWorkThread 和主线程交流的 promise（主线程发）
std::promise<bool>& loadTranslationPromiseEnd();
}
}

#endif // MUSEC_CONTROLLER_APPCONTROLLER
