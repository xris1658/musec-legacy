#ifndef MUSEC_DAO_LOGGINGDAO
#define MUSEC_DAO_LOGGINGDAO

#include <QString>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <memory>

namespace Musec
{
namespace DAO
{
namespace LoggingDAO
{
using LoggerType = std::shared_ptr<spdlog::logger>;

constexpr char loggingFolderName[] = "log";
constexpr char loggingFileSuffix[] = ".log";
// 根据启动时间获取日志文件名
QString loggingFileName();

LoggerType& createLogger();
}
}
}

#endif //MUSEC_DAO_LOGGINGDAO
