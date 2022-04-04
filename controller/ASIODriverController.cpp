#include "ASIODriverController.hpp"

#include "audio/driver/ASIOCallback.hpp"
#include "controller/AppController.hpp"
#include "controller/ConfigController.hpp"
#include "dao/DatabaseDAO.hpp"
#include "event/EventBase.hpp"
#include "event/EventHandler.hpp"
#include "ui/MessageDialog.hpp"
#include "ui/UI.hpp"

namespace Musec::Controller::ASIODriverController
{
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
    auto name = std::get<Musec::Audio::Driver::ASIODriverField::NameField>(driver.driverInfo());
    auto hWnd = reinterpret_cast<HWND>(mainWindow->winId());
    try
    {
        driver->init(hWnd);
    }
    catch(...)
    {
        Musec::UI::MessageDialog::messageDialog(QString("无法加载 ASIO 驱动程序 %1。程序将以未加载驱动的状态运行。").arg(name),
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
    auto info = getASIODriverStreamInfo(driver);
    if(info.outputChannelCount == 0)
    {
        Musec::UI::MessageDialog::messageDialog(QString("当前加载的 ASIO 驱动程序 (%1) 没有输出。").arg(name),
                                             "Musec - 驱动程序警告",
                                             Musec::UI::MessageDialog::IconType::Warning);
    }
    constexpr int inputBufferCount = 64;
    constexpr int outputBufferCount = 64;
    auto& bufferInfo = getASIOBufferInfoList();
    for(int i = 0; i < info.inputChannelCount; ++i)
    {
        bufferInfo[i].isInput = ASIOTrue;
        bufferInfo[i].channelNum = i;
        bufferInfo[i].buffers[0] = bufferInfo[i].buffers[1] = nullptr;
    }
    for(int i = info.inputChannelCount; i < info.inputChannelCount + info.outputChannelCount; ++i)
    {
        bufferInfo[i].isInput = ASIOFalse;
        bufferInfo[i].channelNum = i - info.inputChannelCount;
        bufferInfo[i].buffers[0] = bufferInfo[i].buffers[1] = nullptr;
    }
    //
    auto createBuffersResult = driver->createBuffers(bufferInfo.data(),
                                                     inputBufferCount + outputBufferCount,
                                                     info.preferredBufferSize,
                                                     &getCallbacks());
    if(createBuffersResult != ASE_OK)
    {
        char errorMessageBuffer[124];
        driver->getErrorMessage(errorMessageBuffer);
        // 输出错误字符串
        return;
    }
    auto& channelInfoList = getASIOChannelInfoList();
    for(int i = 0; i < info.inputChannelCount + info.outputChannelCount; ++i)
    {
        channelInfoList[i].channel = bufferInfo[i].channelNum;
        channelInfoList[i].isInput = bufferInfo[i].isInput;
        auto result = driver->getChannelInfo(&(channelInfoList[i]));
        if(result != ASE_OK)
        {
            //
        }
    }
    driver->start();
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
            try
            {
                AppASIODriver() = ASIODriver(item);
                auto& appConfig = ConfigController::appConfig();
                appConfig["musec"]["options"]["audio-hardware"]["driver-id"] =
                        clsid.toStdString();
                ConfigController::saveAppConfig();
            }
            catch(std::runtime_error& exception)
            {
                Musec::UI::MessageDialog::messageDialog("无法加载 ASIO 驱动程序。程序将以未加载音频驱动的方式运行。",
                                                     "Musec - 驱动程序错误",
                                                     Musec::UI::MessageDialog::IconType::Error);
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
    using namespace Audio::Driver;
    AppASIODriver() = ASIODriver();
}

void updateCurrentASIODriverInfo()
{
    using namespace Audio::Driver;
    using namespace UI;
    if(optionsWindow)
    {
        // 获取驱动信息
        auto driverStreamInfo = getASIODriverStreamInfo(AppASIODriver());
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
