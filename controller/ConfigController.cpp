#include "ConfigController.hpp"

namespace Musec::Controller::ConfigController
{
YAML::Node& appConfig()
{
    static YAML::Node ret = Musec::DAO::loadAppConfig(Musec::DAO::ConfigFilePath());
    return ret;
}

void createAppConfig()
{
    auto& configFileNode = appConfig();
    configFileNode["musec"]["options"]["general"]["language"] = "zh-cn";
    configFileNode["musec"]["options"]["general"]["theme"] = "dark";
    configFileNode["musec"]["options"]["general"]["scale"] = 100;
    configFileNode["musec"]["options"]["general"]["touch"] = true;
    configFileNode["musec"]["options"]["general"]["single-instance"] = true;
    configFileNode["musec"]["options"]["general"]["system-render"] = false;
    configFileNode["musec"]["options"]["audio-hardware"]["driver-type"] = "ASIO";
    configFileNode["musec"]["options"]["audio-hardware"]["driver-id"] = "{00000000-0000-0000-0000-000000000000}"; // 无音频驱动
    configFileNode["musec"]["options"]["audio-hardware"]["sample-rate"] = 44100;
    configFileNode["musec"]["options"]["plugin"]["enable-shortcut"] = true;
}

void saveAppConfig()
{
    Musec::DAO::saveAppConfig(appConfig(), Musec::DAO::ConfigFilePath());
}
}
