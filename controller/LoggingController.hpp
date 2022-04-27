#ifndef MUSEC_CONTROLLER_LOGGINGCONTROLLER
#define MUSEC_CONTROLLER_LOGGINGCONTROLLER

#include "dao/LoggingDAO.hpp"

namespace Musec
{
namespace Controller
{
namespace LoggingController
{
Musec::DAO::LoggingDAO::LoggerType& AppLogger();
}
}
}

#endif //MUSEC_CONTROLLER_LOGGINGCONTROLLER
