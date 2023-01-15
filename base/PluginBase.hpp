#ifndef MUSEC_BASE_PLUGINBASE
#define MUSEC_BASE_PLUGINBASE

#include <QList>
#include <QString>

#include <pluginterfaces/base/ipluginbase.h>

namespace Musec
{
namespace Base
{
// UID, name, format, type
using PluginBasicInfo = std::tuple<std::vector<char>, QString, int, int>;

// Path, UID, name, format, type
using PluginWriteInfo = std::tuple<QString, std::vector<char>, QString, int, int>;

// ID, path, UID, name, format, type
using PluginReadInfo = std::tuple<int, QString, std::vector<char>, QString, int, int>;

// VST3 plugin initialization entry
using VST3PluginInitProc = bool(*)();

// VST3 plugin factory entry (GetPluginFactory)
using VST3PluginFactoryProc = Steinberg::IPluginFactory*(*)();

// VST3 plugin uninitialization entry
using VST3PluginExitProc = bool(*)();

enum PluginBasicInfoField
{
    BasicFieldUid = 0,
    BasicFieldName,
    BasicFieldFormat,
    BasicFieldType
};

enum PluginWriteInfoField
{
    WriteFieldPath = 0,
    WriteFieldUid,
    WriteFieldName,
    WriteFieldFormat,
    WriteFieldType
};

enum PluginReadInfoField
{
    ReadFieldId = 0,
    ReadFieldPath,
    ReadFieldUid,
    ReadFieldName,
    ReadFieldFormat,
    ReadFieldType
};

enum PluginFormat
{
    FormatNotAPlugin = 0,
    FormatVST3,
    FormatCLAP,
    PluginFormatCount
};

enum PluginType
{
    TypeUnknown = 0,
    TypeMidiFX,
    TypeInstrument,
    TypeAudioFX
};

PluginFormat pluginFormat(const QString& path);

QList<PluginBasicInfo> scanSingleLibraryFile(const QString& path);

QStringList& defaultPluginDirectoryList();
}
}

#endif // MUSEC_BASE_PLUGINBASE
