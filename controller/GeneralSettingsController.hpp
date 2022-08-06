#ifndef MUSEC_CONTROLLER_GENERALSETTINGSCONTROLLER
#define MUSEC_CONTROLLER_GENERALSETTINGSCONTROLLER

#include <QString>

namespace Musec
{
namespace Controller
{
namespace GeneralSettingsController
{
void setSystemTextRendering(bool value);

void setLanguage(const QString& language);
}
}
}

#endif //MUSEC_CONTROLLER_GENERALSETTINGSCONTROLLER
