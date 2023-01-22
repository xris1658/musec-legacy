#ifndef MUSEC_MODEL_ASIODRIVERLISTMODEL
#define MUSEC_MODEL_ASIODRIVERLISTMODEL

#include "audio/driver/ASIODriver.hpp"
#include "model/ModelBase.hpp"

#include <QAbstractListModel>

#include <vector>

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
    using List = std::vector<ASIODriverBasicInfo>;
public:
    enum RoleNames
    {
        NameRole = Qt::UserRole,
        CLSIDRole,
        RoleEnd
    };
public:
    explicit ASIODriverListModel(QObject* parent = nullptr);
    ~ASIODriverListModel();
public:
    void setList(const List&);
    void setList(List&&);
    List getList() const;
    int itemCount() const;
    static constexpr int columnSize() noexcept;
    Q_INVOKABLE bool empty() const noexcept;
public:
    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex&, int) const override;
protected:
    RoleNamesType roleNames() const override;
private:
    List list_;
    RoleNamesType roleNames_;
};
}
}

#endif // MUSEC_MODEL_ASIODRIVERLISTMODEL
