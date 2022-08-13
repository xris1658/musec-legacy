#include "LoggingDAO.hpp"

#include "base/Base.hpp"
#include "native/Native.hpp"

#include <array>
#include <algorithm>

namespace Musec::DAO::LoggingDAO
{
QString loggingFileName()
{
    // "20220226222547.log"
    auto time = Musec::Native::getLaunchTime();
    auto array = Musec::Native::formatTime(time);
    array[array.size() - 1] = 0;
    // (18-1) - 3 + 5 (-1 + 1)
    constexpr auto length = array.size() - 1 - 3 + Musec::Base::stackArraySize(loggingFileSuffix);
    std::array<char, length> fileNameBuffer = {0};
    std::copy(array.begin(), array.end(), fileNameBuffer.begin());
    std::copy(loggingFileSuffix, loggingFileSuffix + 4, fileNameBuffer.data() + sizeof(array) - 4);
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
