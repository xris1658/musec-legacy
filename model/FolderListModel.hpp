#ifndef MUSEC_MODEL_FOLDERLISTMODEL
#define MUSEC_MODEL_FOLDERLISTMODEL

#include "base/FolderBase.hpp"
#include "model/ModelBase.hpp"

#include <QAbstractListModel>
#include <QList>

namespace Musec
{
namespace Model
{
class FolderListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    using Item = Musec::Base::FolderInfo;
    using List = QList<Item>;
public:
    enum RoleNames
    {
        NameRole = Qt::UserRole,
        PathRole
    };
public:
    explicit FolderListModel(QObject* parent = nullptr);
    virtual ~FolderListModel();
public:
    void setList(const List&);
    void setList(List&&);
    List getList() const;
    Q_INVOKABLE int itemCount() const;
    static constexpr int columnSize();
public:
    virtual int rowCount(const QModelIndex&) const override;
    virtual int columnCount(const QModelIndex&) const override;
    virtual QVariant data(const QModelIndex&, int) const override;
public:
    Q_INVOKABLE QString getPathOfIndex(int index) const;
protected:
    virtual RoleNamesType roleNames() const override;
private:
    List list_;
    RoleNamesType roleNames_;
};
}
}

#endif //MUSEC_MODEL_FOLDERLISTMODEL
