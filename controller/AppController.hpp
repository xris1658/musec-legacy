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
void loadOrInitConfig();

// Between splash screen is opened and main window in opened
void initApplication(Musec::Event::SplashScreen* splashScreen);

bool findAppData();

void initAppData();

void loadAppData();

Musec::Model::PluginListModel& AppPluginList();

Musec::Model::PluginListModel& AppMidiEffectList();

Musec::Model::PluginListModel& AppInstrumentList();

Musec::Model::PluginListModel& AppAudioEffectList();

Musec::Model::ASIODriverListModel& AppASIODriverList();

Musec::Model::AssetDirectoryListModel& AppAssetDirectoryList();

Musec::Model::TranslationFileModel& AppTranslationFileList();

void refreshPluginList(bool rescan);

void loadAssetDirectoryList();

void openSpecialCharacterInput();
}
}

#endif // MUSEC_CONTROLLER_APPCONTROLLER
