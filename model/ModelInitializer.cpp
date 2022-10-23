#include "ModelInitializer.hpp"

#include "base/QmlBase.hpp"
#include "model/ASIODriverListModel.hpp"
#include "model/AssetDirectoryListModel.hpp"
#include "model/AudioTrackSequenceModel.hpp"
#include "model/AutomationModel.hpp"
#include "model/FileListModel.hpp"
#include "model/FolderListModel.hpp"
#include "model/MIDIInputDeviceListModel.hpp"
#include "model/PluginListModel.hpp"
#include "model/TempoAutomationModel.hpp"
#include "model/TrackListModel.hpp"
#include "model/TranslationFileModel.hpp"
#include "model/VolumeFaderScaleModel.hpp"

namespace Musec::Model
{
ModelInitializer::ModelInitializer()
{
    QML_REGISTER_TYPE(ASIODriverListModel,      Musec.Models, 1, 0);
    QML_REGISTER_TYPE(AssetDirectoryListModel,  Musec.Models, 1, 0);
    QML_REGISTER_TYPE(AudioTrackSequenceModel,  Musec.Models, 1, 0);
    QML_REGISTER_TYPE(AutomationModel,          Musec.Models, 1, 0);
    QML_REGISTER_TYPE(FileListModel,            Musec.Models, 1, 0);
    QML_REGISTER_TYPE(FolderListModel,          Musec.Models, 1, 0);
    QML_REGISTER_TYPE(MIDIInputDeviceListModel, Musec.Models, 1, 0);
    QML_REGISTER_TYPE(PluginListModel,          Musec.Models, 1, 0);
    QML_REGISTER_TYPE(PluginSequenceModel,      Musec.Models, 1, 0);
    QML_REGISTER_TYPE(TempoAutomationModel,     Musec.Models, 1, 0);
    QML_REGISTER_TYPE(TrackListModel,           Musec.Models, 1, 0);
    QML_REGISTER_TYPE(TranslationFileModel,     Musec.Models, 1, 0);
    QML_REGISTER_TYPE(VolumeFaderScaleModel,    Musec.Models, 1, 0);
}

void ModelInitializer::initialize()
{
    static ModelInitializer ret;
}
}
