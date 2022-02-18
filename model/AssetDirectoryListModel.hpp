#ifndef MUSEC_MODEL_ASSETDIRECTORYLISTMODEL
#define MUSEC_MODEL_ASSETDIRECTORYLISTMODEL

#include <QAbstractListModel>
#include <QList>

#include "base/AssetDirectoryBase.hpp"

#include "ModelBase.hpp"

namespace Musec
{
namespace Model
{
class AssetDirectoryListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    using Item = Musec::Base::AssetDirectoryInfo;
public:
    enum RoleNames
    {
        IdRole = Qt::UserRole,
        DirectoryRole,
        NameRole
    };
public:
    explicit AssetDirectoryListModel(QObject* parent = nullptr);
    virtual ~AssetDirectoryListModel();
public:
    void setList(const QList<Item>&);
    void setList(QList<Item>&&);
    QList<Item> getList() const;
    int itemCount() const;
    constexpr int columnSize() const;
    void append(const Item& item);
public:
    virtual int rowCount(const QModelIndex&) const override;
    virtual int columnCount(const QModelIndex&) const override;
    virtual QVariant data(const QModelIndex&, int) const override;
protected:
    virtual RoleNamesType roleNames() const override;
private:
    QList<Item> list_;
    RoleNamesType roleNames_;
};
}
}

#endif // MUSEC_MODEL_ASSETDIRECTORYLISTMODEL
