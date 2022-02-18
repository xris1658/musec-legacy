#ifndef MUSEC_MODEL_PLUGINLISTMODEL
#define MUSEC_MODEL_PLUGINLISTMODEL

#include <QAbstractListModel>
#include <QList>

#include "base/PluginBase.hpp"

#include "ModelBase.hpp"

namespace Musec
{
namespace Model
{
class PluginListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    using Item = Musec::Base::PluginReadInfo;
    using List = QList<Item>;
public:
    enum PluginType
    {
        TypeUnknownPlugin,
        TypeMidiFX,
        TypeInstrument,
        TypeAudioFX
    };
    enum RoleNames
    {
        IdRole = Qt::UserRole,
        PathRole,
        UidRole,
        NameRole,
        FormatRole,
        TypeRole
    };
public:
    explicit PluginListModel(QObject* parent = nullptr);
    virtual ~PluginListModel();
public:
    void setList(const List&);
    void setList(List&&);
    List getList() const;
    int itemCount() const;
    constexpr int columnSize() const;
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

#endif // MUSEC_MODEL_PLUGINLISTMODEL
