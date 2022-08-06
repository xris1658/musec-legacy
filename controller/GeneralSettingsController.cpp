#include "GeneralSettingsController.hpp"

#include "controller/ConfigController.hpp"

namespace Musec::Controller::GeneralSettingsController
{
void setSystemTextRendering(bool value)
{
    using namespace Musec::Controller::ConfigController;
    appConfig()["musec"]["options"]["general"]["system-render"] = value;
    saveAppConfig();
}

void setLanguage(const QString& language)
{
    using namespace Musec::Controller::ConfigController;
    appConfig()["musec"]["options"]["general"]["language"] = language.toStdString();
    saveAppConfig();
}
}
