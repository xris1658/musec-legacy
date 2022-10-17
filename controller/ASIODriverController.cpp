#include "ASIODriverController.hpp"

#include "audio/driver/ASIODriver.hpp"
#include "audio/driver/Literals.hpp"
#include "concurrent/ButlerThread.hpp"
#include "controller/AppController.hpp"
#include "controller/ConfigController.hpp"
#include "ui/MessageDialog.hpp"
#include "ui/UI.hpp"

#include <QDebug>

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
        Musec::UI::MessageDialog::messageDialog(
            Musec::UI::strings->property("loadASIODriverErrorTextWithErrorCode").value<QString>()
                .arg(name)
                .arg(error)
                .arg(Musec::Audio::Driver::Literals::asioErrorMessage(error)),
            Musec::UI::strings->property("driverWarningTitleText").value<QString>(),
            Musec::UI::MessageDialog::IconType::Warning);
    }
    else
    {
        Musec::UI::MessageDialog::messageDialog(
            Musec::UI::strings->property("loadASIODriverErrorTextWithErrorCodeAndInfo").value<QString>()
                .arg(name)
                .arg(error)
                .arg(Musec::Audio::Driver::Literals::asioErrorMessage(error))
                .arg(errorMessageBuffer),
            Musec::UI::strings->property("driverWarningTitleText").value<QString>(),
            Musec::UI::MessageDialog::IconType::Warning);
    }
}
}

void loadASIODriver()
{
    using namespace Audio::Driver;
    using namespace UI;
    auto& driver = AppASIODriver();
    Musec::Concurrent::ButlerThread::instance();
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
        Musec::UI::MessageDialog::messageDialog(
            Musec::UI::strings->property("loadASIODriverErrorTextWithInfo").value<QString>()
                .arg(name, errorString),
            Musec::UI::strings->property("driverWarningTitleText").value<QString>(),
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
        Musec::UI::MessageDialog::messageDialog(
            Musec::UI::strings->property("loadASIODriverNoOutputText").value<QString>()
                .arg(name),
            Musec::UI::strings->property("driverWarningTitleText").value<QString>(),
            Musec::UI::MessageDialog::IconType::Warning);
        return;
    }
    allocateASIODriverBuffer();
    startASIODriver();
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
                    Musec::UI::strings->property("loadASIODriverErrorText").value<QString>(),
                    Musec::UI::strings->property("driverErrorTitleText").value<QString>(),
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
    mainWindow->setProperty("engineRunning", QVariant::fromValue<bool>(false));
    AppASIODriver() = ASIODriver();
}

void updateCurrentASIODriverInfo()
{
    using namespace Musec::Audio::Driver;
    using namespace UI;
    auto& driver = AppASIODriver();
    auto info = Musec::Audio::Driver::getChannelCount(driver);
    auto& channelInfoList = getASIOChannelInfoList();
    auto& bufferInfo = getASIOBufferInfoList();
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
    inputChannelInfoList().setList(channelInfoList.data(), info.inputCount);
    outputChannelInfoList().setList(channelInfoList.data() + info.inputCount, info.outputCount);
    if(optionsWindow)
    {
        const auto& driverStreamInfo = getASIODriverStreamInfo(AppASIODriver());
        optionsWindow->setProperty("bufferSize",
                                   QVariant::fromValue<int>(driverStreamInfo.preferredBufferSize));
        optionsWindow->setProperty("inputLatencyInSamples",
                                   QVariant::fromValue<int>(driverStreamInfo.inputLatencyInSamples));
        optionsWindow->setProperty("outputLatencyInSamples",
                                   QVariant::fromValue<int>(driverStreamInfo.outputLatencyInSamples));
        optionsWindow->setProperty("sampleRate",
                                   QVariant::fromValue<double>(driverStreamInfo.sampleRate));
        optionsWindow->setProperty("outputChannelList",
                                   QVariant::fromValue<QObject*>(&outputChannelInfoList()));
        optionsWindow->setProperty("leftOutputChannel",
                                   QVariant::fromValue<int>(0));
        optionsWindow->setProperty("rightOutputChannel",
                                   QVariant::fromValue<int>(1));
    }
}

void allocateASIODriverBuffer()
{
    using namespace Musec::Audio::Driver;
    auto& driver = AppASIODriver();
    auto& bufferInfo = getASIOBufferInfoList();
    auto info = Musec::Audio::Driver::getChannelCount(driver);
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
    auto createBuffersResult = driver->createBuffers(
        bufferInfo.data(),
        info.inputCount + info.outputCount,
        Musec::Audio::Driver::getBufferSize(driver).preferredBufferSize,
        &getCallbacks());
    if(createBuffersResult != ASE_OK)
    {
        Impl::showASIOErrorMessageDialog(driver, createBuffersResult);
        return;
    }
    updateCurrentASIODriverInfo();
}

void startASIODriver()
{
    using namespace Musec::Audio::Driver;
    using namespace Musec::UI;
    auto& driver = AppASIODriver();
    Musec::Audio::Driver::driverSupportsOutputReady = (driver->outputReady() == ASE_OK);
    auto startResult = driver->start();
    if(startResult != ASE_OK)
    {
        Impl::showASIOErrorMessageDialog(driver, startResult);
    }
    mainWindow->setProperty("engineRunning", QVariant::fromValue<bool>(true));
}

Musec::Model::ASIOChannelInfoListModel& inputChannelInfoList()
{
    static Musec::Model::ASIOChannelInfoListModel ret;
    return ret;
}

Musec::Model::ASIOChannelInfoListModel& outputChannelInfoList()
{
    static Musec::Model::ASIOChannelInfoListModel ret;
    return ret;
}

void resetASIODriver()
{
    const auto& info = Musec::Audio::Driver::AppASIODriver().driverInfo();
    setASIODriver(std::get<Musec::Audio::Driver::ASIODriverField::CLSIDField>(info));
}

}
