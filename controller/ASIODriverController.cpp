#include "ASIODriverController.hpp"

#include "audio/driver/ASIODriver.hpp"
#include "audio/driver/ASIOErrorText.hpp"
#include "controller/AppController.hpp"
#include "controller/AudioEngineController.hpp"
#include "controller/ConfigController.hpp"
#include "native/Native.hpp"
#include "ui/MessageDialog.hpp"
#include "ui/UI.hpp"

#include <QDebug>

#include <vector>

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
                .arg(Musec::Audio::Driver::getASIOErrorText(error)),
            Musec::UI::strings->property("driverWarningTitleText").value<QString>(),
            Musec::UI::MessageDialog::IconType::Warning);
    }
    else
    {
        Musec::UI::MessageDialog::messageDialog(
            Musec::UI::strings->property("loadASIODriverErrorTextWithErrorCodeAndInfo").value<QString>()
                .arg(name)
                .arg(error)
                .arg(Musec::Audio::Driver::getASIOErrorText(error))
                .arg(errorMessageBuffer),
            Musec::UI::strings->property("driverWarningTitleText").value<QString>(),
            Musec::UI::MessageDialog::IconType::Warning);
    }
}
}

bool loadASIODriver()
{
    using namespace Musec::Audio::Driver;
    using namespace Musec::UI;
    auto& driver = AppASIODriver();
    inputChannelInfoList().setList(nullptr, 0);
    outputChannelInfoList().setList(nullptr, 0);
    if(!driver)
    {
        return false;
    }
    auto hWnd = reinterpret_cast<Musec::Native::WindowType>(mainWindow->winId());
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
        return false;
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
        return false;
    }
    return true;
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
            auto oldMaxSize = Musec::Audio::Driver::getBufferSize(AppASIODriver()).preferredBufferSize;
            unloadASIODriver();
            try
            {
                AppASIODriver() = ASIODriver(item);
                if(loadASIODriver())
                {
                    auto newMaxSize = Musec::Audio::Driver::getBufferSize(AppASIODriver()).preferredBufferSize;
                    Musec::Controller::AudioEngineController::AppProject().reallocateAudioBuffer(newMaxSize);
                    auto& appConfig = ConfigController::appConfig();
                    appConfig["musec"]["options"]["audio-hardware"]["driver-id"] =
                        clsid.toStdString();
                    ConfigController::saveAppConfig();
                    if(allocateASIODriverBuffer())
                    {
                        if(startASIODriver())
                        {
                            //
                        }
                    }
                }
            }
            catch(std::runtime_error&)
            {
                Musec::UI::MessageDialog::messageDialog(
                    Musec::UI::strings->property("loadASIODriverErrorText").value<QString>(),
                    Musec::UI::strings->property("driverErrorTitleText").value<QString>(),
                    Musec::UI::MessageDialog::IconType::Error);
                unloadASIODriver();
            }
            return;
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
    using namespace Musec::UI;
    using namespace Audio::Driver;
    mainWindow->setProperty("engineRunning", QVariant::fromValue<bool>(false));
    AppASIODriver() = ASIODriver();
    inputChannelInfoList().setList(nullptr, 0);
    outputChannelInfoList().setList(nullptr, 0);
}

bool updateCurrentASIODriverInfo()
{
    using namespace Musec::Audio::Driver;
    using namespace Musec::UI;
    auto& driver = AppASIODriver();
    prepareChannelInfo(driver);
    auto& channelInfoList = getASIOChannelInfoList();
    std::vector<bool> channelInfoValid(channelInfoList.size(), false);
    for(int i = 0; i < channelInfoList.size(); ++i)
    {
        auto* channelInfo = channelInfoList.data() + i;
        auto getChannelInfoResult = driver->getChannelInfo(channelInfo);
        if(getChannelInfoResult == ASE_OK)
        {
            channelInfoValid[i] = true;
        }
    }
    // TODO: Use `channelInfoValid`
    auto [inputCount, outputCount] = getChannelCount(driver);
    auto [minSize, maxSize, preferredSize, granularity] = getBufferSize();
    auto [inputLatency, outputLatency] = getLatency();
    fillOutput1WithZero(preferredSize);
    inputChannelInfoList().setList(channelInfoList.data(), inputCount);
    outputChannelInfoList().setList(channelInfoList.data() + inputCount, outputCount);
    if(optionsWindow)
    {
        optionsWindow->setProperty("bufferSize",
            QVariant::fromValue<int>(preferredSize));
        optionsWindow->setProperty("inputLatencyInSamples",
            QVariant::fromValue<int>(inputLatency));
        optionsWindow->setProperty("outputLatencyInSamples",
            QVariant::fromValue<int>(outputLatency));
        optionsWindow->setProperty("sampleRate",
            QVariant::fromValue<double>(getSampleRate()));
        optionsWindow->setProperty("inputList",
            QVariant::fromValue<QObject*>(&inputChannelInfoList()));
        optionsWindow->setProperty("outputList",
            QVariant::fromValue<QObject*>(&outputChannelInfoList()));
    }
    return true;
}

bool allocateASIODriverBuffer()
{
    using namespace Musec::Audio::Driver;
    auto& driver = AppASIODriver();
    prepareBufferInfo(driver);
    auto& bufferInfoList = getASIOBufferInfoList();
    auto createBufferResult = driver->createBuffers(
        bufferInfoList.data(),
        bufferInfoList.size(),
        getBufferSize(driver).preferredBufferSize,
        &getCallbacks()
        );
    if(createBufferResult != ASE_OK)
    {
        Impl::showASIOErrorMessageDialog(driver, createBufferResult);
        return false;
    }
    updateCurrentASIODriverInfo();
    return true;
}

bool startASIODriver()
{
    using namespace Musec::Audio::Driver;
    using namespace Musec::UI;
    auto& driver = AppASIODriver();
    Musec::Audio::Driver::driverSupportsOutputReady = (driver->outputReady() == ASE_OK);
    auto startResult = driver->start();
    if(startResult != ASE_OK)
    {
        Impl::showASIOErrorMessageDialog(driver, startResult);
        return false;
    }
    mainWindow->setProperty("engineRunning", QVariant::fromValue<bool>(true));
    return true;
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
