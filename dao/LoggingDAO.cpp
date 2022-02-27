#include "LoggingDAO.hpp"

#include "native/Native.hpp"

#include <array>
#include <cstring>
#include <string>

namespace Musec::DAO::LoggingDAO
{
const QString loggingFileName()
{
    // "20220226222547.log"
    constexpr char suffix[] = ".log";
    auto time = Musec::Native::getLaunchTime();
    auto array = Musec::Native::formatTime(time);
    constexpr auto length = array.size() - 1 - 3 + sizeof(loggingFileSuffix) - 1;
    std::array<char, length> fileNameBuffer = {0};
    std::strcpy(fileNameBuffer.data(), array.data());
    std::strcpy(fileNameBuffer.data() + sizeof(array) - 1 - 3, loggingFileSuffix);
    return QString::fromLatin1(fileNameBuffer.data());
}

LoggerType& createLogger()
{
    static auto ret =  spdlog::basic_logger_mt(
        "MusecLogger",
        QString(
            Musec::Native::DataDirectoryPath() + "\\log\\" + loggingFileName()
        )
        .toStdString()
    );
    return ret;
}
}
