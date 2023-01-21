#ifndef MUSEC_BASE_PLUGINBASE
#define MUSEC_BASE_PLUGINBASE

#include <QList>
#include <QString>

#include <pluginterfaces/base/ipluginbase.h>

#include <vector>

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

std::vector<PluginBasicInfo> scanSingleLibraryFile(const QString& path);
}
}

#endif // MUSEC_BASE_PLUGINBASE
