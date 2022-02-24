#ifndef MUSEC_MODEL_ASIODRIVERLISTMODEL
#define MUSEC_MODEL_ASIODRIVERLISTMODEL

#include "audio/driver/ASIODriver.hpp"
#include "model/ModelBase.hpp"

#include <QAbstractListModel>
#include <QList>

namespace Musec
{
namespace Model
{
using namespace Musec::Audio::Driver;
class ASIODriverListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    using Item = ASIODriverBasicInfo;
    using List = QList<ASIODriverBasicInfo>;
public:
    enum RoleNames
    {
        NameRole = Qt::UserRole,
        CLSIDRole
    };
public:
    explicit ASIODriverListModel(QObject* parent = nullptr);
    virtual ~ASIODriverListModel();
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

#endif // MUSEC_MODEL_ASIODRIVERLISTMODEL
