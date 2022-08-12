#include "LoggingDAO.hpp"

#include "base/Base.hpp"
#include "native/Native.hpp"

#include <array>
#include <cstring>

namespace Musec::DAO::LoggingDAO
{
QString loggingFileName()
{
    // "20220226222547.log"
    auto time = Musec::Native::getLaunchTime();
    auto array = Musec::Native::formatTime(time);
    constexpr auto length = array.size() - 1 - 3 + Musec::Base::stackArraySize(loggingFileSuffix) - 1;
    std::array<char, length> fileNameBuffer = {0};
    std::copy(array.begin(), array.end(), fileNameBuffer.begin());
    std::strcpy(fileNameBuffer.data() + sizeof(array) - 1 - 3, loggingFileSuffix);
    return QString::fromLatin1(fileNameBuffer.data());
}

LoggerType& createLogger()
{
    static auto ret =  spdlog::basic_logger_mt(
        "MusecLogger",
        QString(
            Musec::Native::dataDirectoryPath() + "\\log\\" + loggingFileName()
        )
        .toStdString()
    );
    return ret;
}
}
