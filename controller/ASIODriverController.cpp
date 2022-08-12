#include "ASIODriverController.hpp"

#include "audio/driver/ASIOCallback.hpp"
#include "audio/driver/ASIODriver.hpp"
#include "audio/driver/Literals.hpp"
#include "controller/AppController.hpp"
#include "controller/ConfigController.hpp"
#include "dao/DatabaseDAO.hpp"
#include "event/EventHandler.hpp"
#include "ui/MessageDialog.hpp"
#include "ui/UI.hpp"

namespace Musec::Controller::ASIODriverController
{
namespace Impl
{
void showASIOErrorMessageDialog(Musec::Audio::Driver::ASIODriver& driver, ASIOError error)
{
    auto name = std::get<Musec::Audio::Driver::ASIODriverField::NameField>(driver.driverInfo());
    char errorMessageBuffer[124] = {0};
    driver->getErrorMessage(errorMessageBuffer);
    if(errorMessageBuffer[0] == 0)
    {
        Musec::UI::MessageDialog::messageDialog(QString("无法加载 ASIO 驱动程序 %1。\n错误代码 %2：%3")
                .arg(name)
                .arg(error)
                .arg(Musec::Audio::Driver::Literals::asioErrorMessage(error)),
            "Musec - 驱动程序警告",
            Musec::UI::MessageDialog::IconType::Warning);
    }
    else
    {
        Musec::UI::MessageDialog::messageDialog(QString("无法加载 ASIO 驱动程序 %1。\n错误代码：%2：%3\n\n以下是驱动程序的错误信息：\n %4")
                .arg(name)
                .arg(error)
                .arg(Musec::Audio::Driver::Literals::asioErrorMessage(error))
                .arg(errorMessageBuffer),
            "Musec - 驱动程序警告",
            Musec::UI::MessageDialog::IconType::Warning);
    }
}
}
void loadASIODriver()
{
    using namespace Audio::Driver;
    // using namespace Musec::DAO;
    using namespace UI;
    auto& driver = AppASIODriver();
    if(!driver)
    {
        return;
    }
    auto hWnd = reinterpret_cast<HWND>(mainWindow->winId());
    if(driver->init(hWnd) == ASIOFalse)
    {
        auto name = std::get<Musec::Audio::Driver::ASIODriverField::NameField>(driver.driverInfo());
        char errorString[124];
        std::memset(errorString, 0, 124);
        driver->getErrorMessage(errorString);
        Musec::UI::MessageDialog::messageDialog(QString("无法加载 ASIO 驱动程序 %1。程序将以未加载驱动的状态运行。\n\n以下是驱动程序的错误信息：\n %2").arg(name, errorString),
            "Musec - 驱动程序警告",
            Musec::UI::MessageDialog::IconType::Warning);
    }
    auto& appConfig = ConfigController::appConfig();
    auto sampleRate = appConfig["musec"]["options"]["audio-hardware"]["sample-rate"].as<double>();
    // auto bufferSize = 0;
    // appConfig["musec"]["options"]["audio-hardware"]["buffer-size"].as<int>();
    auto canSampleRate = driver->canSampleRate(sampleRate);
    if(canSampleRate == ASE_OK || canSampleRate == ASE_SUCCESS)
    {
        driver->setSampleRate(sampleRate);
    }
    else
    {
        driver->setSampleRate(44100.0);
        appConfig["musec"]["options"]["audio-hardware"]["sample-rate"] = 44100.0;
        ConfigController::saveAppConfig();
    }
    auto info = Musec::Audio::Driver::getChannelCount(driver);
    if(info.outputCount == 0)
    {
        auto name = std::get<Musec::Audio::Driver::ASIODriverField::NameField>(driver.driverInfo());
        Musec::UI::MessageDialog::messageDialog(QString("当前加载的 ASIO 驱动程序 (%1) 没有输出。").arg(name),
                                             "Musec - 驱动程序警告",
                                             Musec::UI::MessageDialog::IconType::Warning);
        return;
    }
    auto& bufferInfo = getASIOBufferInfoList();
    for(int i = 0; i < info.inputCount; ++i)
    {
        bufferInfo[i].isInput = ASIOTrue;
        bufferInfo[i].channelNum = i;
        bufferInfo[i].buffers[0] = bufferInfo[i].buffers[1] = nullptr;
    }
    for(int i = info.inputCount; i < info.inputCount + info.outputCount; ++i)
    {
        bufferInfo[i].isInput = ASIOFalse;
        bufferInfo[i].channelNum = i - info.inputCount;
        bufferInfo[i].buffers[0] = bufferInfo[i].buffers[1] = nullptr;
    }
    //
    auto createBuffersResult = driver->createBuffers(bufferInfo.data(),
                                                     info.inputCount + info.outputCount,
                                                     Musec::Audio::Driver::getBufferSize(driver).preferredBufferSize,
                                                     &getCallbacks());
    if(createBuffersResult != ASE_OK)
    {
        Impl::showASIOErrorMessageDialog(driver, createBuffersResult);
        return;
    }
    auto& channelInfoList = getASIOChannelInfoList();
    for(int i = 0; i < info.inputCount + info.outputCount; ++i)
    {
        channelInfoList[i].channel = bufferInfo[i].channelNum;
        channelInfoList[i].isInput = bufferInfo[i].isInput;
        auto getChannelInfoResult = driver->getChannelInfo(&(channelInfoList[i]));
        if(getChannelInfoResult != ASE_OK)
        {
            Impl::showASIOErrorMessageDialog(driver, getChannelInfoResult);
        }
    }
    Musec::Audio::Driver::driverSupportsOutputReady = (driver->outputReady() == ASE_OK);
    auto startResult = driver->start();
    if(startResult != ASE_OK)
    {
        Impl::showASIOErrorMessageDialog(driver, startResult);
    }
    mainWindow->setProperty("engineRunning", QVariant::fromValue<bool>(true));
    updateCurrentASIODriverInfo();
}

QString getASIODriver()
{
    auto& appConfig = ConfigController::appConfig();
    return QString::fromStdString(
            appConfig["musec"]["options"]["audio-hardware"]["driver-id"].as<std::string>()
    );
}

void setASIODriver(const QString& clsid)
{
    using namespace Audio::Driver;
    auto driverList = AppASIODriverList().getList();
    for(auto& item: driverList)
    {
        auto& itemCLSID = std::get<CLSIDField>(item);
        if(itemCLSID.compare(clsid, Qt::CaseInsensitive) == 0)
        {
            // 防止两个驱动同时加载时同时抢占声卡独占权
            // 参考：https://github.com/dechamps/FlexASIO/issues/86
            unloadASIODriver();
            try
            {
                AppASIODriver() = ASIODriver(item);
                loadASIODriver();
                auto& appConfig = ConfigController::appConfig();
                appConfig["musec"]["options"]["audio-hardware"]["driver-id"] =
                        clsid.toStdString();
                ConfigController::saveAppConfig();
            }
            catch(std::runtime_error&)
            {
                Musec::UI::MessageDialog::messageDialog(
                    "无法加载 ASIO 驱动程序。程序将以未加载音频驱动的方式运行。",
                    "Musec - 驱动程序错误",
                    Musec::UI::MessageDialog::IconType::Error);
                unloadASIODriver();
            }
            break;
        }
    }
}

void openASIODriverControlPanel()
{
    using namespace Audio::Driver;
    if(AppASIODriver())
    {
        AppASIODriver()->controlPanel();
    }
}

void unloadASIODriver()
{
    using namespace UI;
    using namespace Audio::Driver;
    AppASIODriver() = ASIODriver();
    mainWindow->setProperty("engineRunning", QVariant::fromValue<bool>(false));
}

void updateCurrentASIODriverInfo()
{
    using namespace Audio::Driver;
    using namespace UI;
    if(optionsWindow)
    {
        // 获取驱动信息
        const auto& driverStreamInfo = getASIODriverStreamInfo(AppASIODriver());
        optionsWindow->setProperty("bufferSize",
                                   QVariant::fromValue<int>(driverStreamInfo.preferredBufferSize));
        optionsWindow->setProperty("inputLatencyInSamples",
                                   QVariant::fromValue<int>(driverStreamInfo.inputLatencyInSamples));
        optionsWindow->setProperty("outputLatencyInSamples",
                                   QVariant::fromValue<int>(driverStreamInfo.outputLatencyInSamples));
        optionsWindow->setProperty("sampleRate",
                                   QVariant::fromValue<double>(driverStreamInfo.sampleRate));
    }
}

}
