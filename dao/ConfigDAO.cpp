#include "ConfigDAO.hpp"

#include "native/Native.hpp"

#include <yaml-cpp/yaml.h>
#include <yaml-cpp/emitter.h>
#include <yaml-cpp/parser.h>

#include <fstream>

namespace Musec::DAO
{
const QString& ConfigFilePath()
{
    using namespace Musec::Native;
    static QString ret = QString(DataDirectoryPath()).append(configFileNameWithBackslash);
    return ret;
}

void saveAppConfig(const YAML::Node& node, const QString& path)
{
    std::ofstream ofs(path.toStdString().c_str());
    ofs << node;
    ofs.close();
}

YAML::Node loadAppConfig(const QString& path)
{
    try
    {
        auto configFile = YAML::LoadFile(path.toStdString());
        return configFile;
    }
    catch (YAML::BadFile&)
    {
        //
    }
    return YAML::Node();
}

}
