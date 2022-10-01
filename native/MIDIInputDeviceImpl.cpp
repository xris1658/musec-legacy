#include "MIDIInputDeviceImpl.hpp"

#include <winrt/base.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Devices.h>
#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Windows.Devices.Midi.h>

namespace Musec::Native
{
QList<MIDIInputDeviceInfo> enumerateMIDIInputDeviceInfo()
{
    using namespace winrt;
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::Devices;
    using namespace winrt::Windows::Devices::Enumeration;
    using namespace winrt::Windows::Devices::Midi;
    winrt::init_apartment();
    QList<MIDIInputDeviceInfo> ret;
    const auto& inputDevices = DeviceInformation::FindAllAsync(MidiInPort::GetDeviceSelector()).get();
    auto inputDeviceCount = inputDevices.Size();
    ret.reserve(inputDeviceCount);
    for(decltype(inputDeviceCount) i = 0; i < inputDeviceCount; ++i)
    {
        const auto& device = inputDevices.GetAt(i);
        ret.append({
            QString::fromWCharArray(device.Name().c_str()),
            QString::fromWCharArray(device.Id().c_str())
        });
    }
    return ret;
}

MIDIInputDeviceImpl::MIDIInputDeviceImpl(): midiInPort_(nullptr)
{
}

MIDIInputDeviceImpl::MIDIInputDeviceImpl(const MIDIInputDeviceInfo& info):
    midiInPort_(winrt::Windows::Devices::Midi::MidiInPort::FromIdAsync(
        std::get<MIDIInputDeviceInfoField::IdField>(info).toStdWString()
    ).get())
{
}

MIDIInputDeviceImpl::MIDIInputDeviceImpl(MIDIInputDeviceImpl&& rhs) noexcept:
    midiInPort_(std::move(rhs.midiInPort_))
{
}

MIDIInputDeviceImpl& MIDIInputDeviceImpl::operator=(MIDIInputDeviceImpl&& rhs) noexcept
{
    if(this != &rhs)
    {
        midiInPort_ = std::move(rhs.midiInPort_);
    }
    return *this;
}

MIDIInputDeviceImpl::~MIDIInputDeviceImpl()
{
    midiInPort_ = {nullptr};
    winrt::uninit_apartment();
}

QString MIDIInputDeviceImpl::deviceId() const
{
    using namespace winrt::Windows::Devices::Midi;
    return QString::fromWCharArray(
        midiInPort_.DeviceId().c_str()
    );
}
}