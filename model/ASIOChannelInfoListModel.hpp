#ifndef MUSEC_MODEL_ASIOCHANNELINFOLISTMODEL
#define MUSEC_MODEL_ASIOCHANNELINFOLISTMODEL

#include "audio/driver/ASIODriver.hpp"
#include "model/ModelBase.hpp"

#include <QAbstractListModel>
#include <QList>

namespace Musec
{
namespace Model
{
class ASIOChannelInfoListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    enum RoleNames
    {
        ChannelIndexRole = Qt::UserRole, // ASIOChannelInfo::channel
        IsInputRole,                     // ASIOChannelInfo::isInput
        IsActiveRole,                    // ASIOChannelInfo::isActive
        GroupRole,                       // ASIOChannelInfo::channelGroup
        SampleTypeRole,                  // ASIOChannelInfo::type
        NameRole,                        // ASIOChannelInfo::name
        RoleEnd
    };
public:
    explicit ASIOChannelInfoListModel(QObject* parent = nullptr);
    ~ASIOChannelInfoListModel();
public:
    void setList(ASIOChannelInfo* const channelInfoList, int channelCount);
    ASIOChannelInfo* getList() const;
    int itemCount() const;
    static constexpr int columnSize();
    Q_INVOKABLE bool empty() const noexcept;
public:
    virtual int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;
protected:
    RoleNamesType roleNames() const override;
private:
    ASIOChannelInfo* channelInfoList_;
    int channelCount_;
    RoleNamesType roleNames_;
};
}
}

#endif //MUSEC_MODEL_ASIOCHANNELINFOLISTMODEL
