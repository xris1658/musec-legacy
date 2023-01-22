#ifndef MUSEC_MODEL_PLUGINLISTMODEL
#define MUSEC_MODEL_PLUGINLISTMODEL

#include "base/PluginBase.hpp"
#include "model/ModelBase.hpp"

#include <QAbstractListModel>
#include <QList>

#include <vector>

namespace Musec
{
namespace Model
{
class PluginListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    using Item = Musec::Base::PluginReadInfo;
    using List = std::vector<Item>;
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
        NameRole,
        FormatRole,
        TypeRole,
        RoleEnd
    };
public:
    explicit PluginListModel(QObject* parent = nullptr);
    ~PluginListModel();
public:
    void setList(const List&);
    void setList(List&&);
    List getList() const;
    int itemCount() const;
    static constexpr int columnSize() noexcept;
public:
    virtual int rowCount(const QModelIndex&) const override;
    virtual int columnCount(const QModelIndex&) const override;
    virtual QVariant data(const QModelIndex&, int) const override;
protected:
    virtual RoleNamesType roleNames() const override;
private:
    List list_;
};
}
}

#endif // MUSEC_MODEL_PLUGINLISTMODEL
