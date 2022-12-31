#ifndef MUSEC_MODEL_MIDIINPUTDEVICELISTMODEL
#define MUSEC_MODEL_MIDIINPUTDEVICELISTMODEL

#include "audio/midi/MIDIInputDevice.hpp"
#include "model/ModelBase.hpp"

#include <QAbstractListModel>
#include <QList>

namespace Musec
{
namespace Model
{
class MIDIInputDeviceListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    using Item = Musec::Audio::MIDI::MIDIInputDeviceInfo;
    using List = QList<Item>;
public:
    enum RoleNames
    {
        NameRole = Qt::UserRole,
        IdRole,
        RoleEnd
    };
public:
    explicit MIDIInputDeviceListModel(QObject* parent = nullptr);
    ~MIDIInputDeviceListModel() override;
public:
    void setList(const List& list);
    void setList(List&& list);
    List getList() const;
    int itemCount() const;
    static constexpr int columnSize() noexcept;
    Q_INVOKABLE bool empty() const noexcept;
public:
    virtual int rowCount(const QModelIndex&) const override;
    virtual int columnCount(const QModelIndex&) const override;
    virtual QVariant data(const QModelIndex&, int) const override;
protected:
    virtual RoleNamesType roleNames() const override;
private:
    List list_;
    RoleNamesType roleNames_;
};
}
}

#endif //MUSEC_MODEL_MIDIINPUTDEVICELISTMODEL
