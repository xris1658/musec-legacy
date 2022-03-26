#ifndef MUSEC_AUDIO_DRIVER_ASIODRIVER
#define MUSEC_AUDIO_DRIVER_ASIODRIVER

#include "audio/driver/ASIODriverForward.hpp"
#include "audio/driver/ASIODriverStreamInfo.hpp"

#include <QString>
#include <QList>

#include <tuple>
#include <memory>

namespace Musec
{
namespace Audio
{
namespace Driver
{
using ASIODriverBasicInfo = std::tuple<QString, QString>;

// CLSID 字符串的长度（32 位数据 + 4 个横杠 + 2 个花括号，不包含 NULL 终止符）
constexpr auto CLSIDStringLength = 38;

constexpr int inputChannelCount = 64;
constexpr int outputChannelCount = 64;
constexpr int channelCount = inputChannelCount + outputChannelCount;
using ASIOBufferInfoList = std::array<ASIOBufferInfo, channelCount>;
using ASIOChannelInfoList = std::array<ASIOChannelInfo, channelCount>;

ASIOBufferInfoList& getASIOBufferInfoList();

ASIOChannelInfoList& getASIOChannelInfoList();

enum ASIODriverField
{
    NameField = 0,
    CLSIDField
};

QList<ASIODriverBasicInfo> enumerateDrivers();

class ASIODriver
{
public:
    ASIODriver();
    explicit ASIODriver(const ASIODriverBasicInfo& info);
    ASIODriver(const ASIODriver&) = delete;
    ASIODriver& operator=(const ASIODriver&) = delete;
    ASIODriver(ASIODriver&&) noexcept;
    ASIODriver& operator=(ASIODriver&&) noexcept;
    ~ASIODriver();
public:
    ASIODriverBasicInfo driverInfo() const;
    inline IASIO* driver() const;
    operator IASIO*() const;
    IASIO* operator*() const;
    IASIO* operator->() const;
public:
    // static QList<ASIODriverBasicInfo> enumerateDrivers();
private:
    ASIODriverBasicInfo driverInfo_;
    IASIO* driver_; // 有必要上 shared_ptr 吗？
};

// ASIO 驱动
// 注意，由于 IASIO 需要接收窗口的 handle 才能初始化（程序选择的是主窗口，
// 参见 Musec::Event::EventHandler::onMainWindowOpened()），因此需要在主窗口关闭前
// 让 IASIO 正常退出，然后再关闭主窗口。否则主窗口比 ASIO 销毁早，从而导致 ASIO
// 退出时出现问题，有可能导致程序无法退出，或者程序崩溃。
ASIODriver& AppASIODriver();

ASIODriverStreamInfo getASIODriverStreamInfo(const ASIODriver& driver);
}
}
}

#endif // MUSEC_AUDIO_DRIVER_ASIODRIVER
