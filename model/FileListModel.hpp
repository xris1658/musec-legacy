#ifndef MUSEC_MODEL_FILELISTMODEL
#define MUSEC_MODEL_FILELISTMODEL

#include "base/FileBase.hpp"
#include "model/ModelBase.hpp"

#include <QAbstractListModel>
#include <QList>

namespace Musec
{
namespace Model
{
class FileListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    using Item = Musec::Base::FileInfo;
    using List = QList<Item>;
public:
    enum RoleNames
    {
        NameRole = Qt::UserRole,
        PathRole
    };
public:
    explicit FileListModel(QObject* parent = nullptr);
    virtual ~FileListModel();
public:
    void setList(const List&);
    void setList(List&&);
    List getList() const;
    int itemCount() const;
    static constexpr int columnSize();
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

#endif //MUSEC_MODEL_FILELISTMODEL