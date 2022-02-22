#ifndef MUSEC_CONTROLLER_CONFIGCONTROLLER
#define MUSEC_CONTROLLER_CONFIGCONTROLLER

#include "dao/ConfigDAO.hpp"

namespace Musec
{
namespace Controller
{
namespace ConfigController
{
YAML::Node& appConfig();

void createAppConfig();

void saveAppConfig();
}
}
}

#endif //MUSEC_CONTROLLER_CONFIGCONTROLLER
