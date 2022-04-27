#include "LoggingController.hpp"

namespace Musec::Controller::LoggingController
{
Musec::DAO::LoggingDAO::LoggerType& AppLogger()
{
    return DAO::LoggingDAO::createLogger();
}
}
