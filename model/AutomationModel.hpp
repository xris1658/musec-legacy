#ifndef MUSEC_MODEL_AUTOMATIONMODEL
#define MUSEC_MODEL_AUTOMATIONMODEL

#include "audio/base/AutomationWithCurve.hpp"
#include "model/ModelBase.hpp"

#include <QAbstractListModel>

namespace Musec
{
namespace Model
{
class AutomationModel: public QAbstractListModel
{
    Q_OBJECT
public:
    using Automation = Musec::Audio::Base::AutomationWithCurve<std::int64_t, double>;
    using Point = Automation::Point;
public:
    enum RoleNames
    {
        TimeRole = Qt::UserRole,
        ValueRole,
        CurveRole
    };
public:
    explicit AutomationModel(QObject* parent = nullptr);
    virtual ~AutomationModel();
public:
    void setAutomation(const Automation&);
    void setAutomation(Automation&&);
    const Automation& getAutomation() const;
    int pointCount() const;
    static constexpr int columnSize();
public:
    virtual int rowCount(const QModelIndex&) const override;
    virtual int columnCount(const QModelIndex&) const override;
    virtual QVariant data(const QModelIndex&, int) const override;
protected:
    virtual RoleNamesType roleNames() const override;
private:
    Automation automation_;
    RoleNamesType roleNames_;
};
}
}

#endif //MUSEC_MODEL_AUTOMATIONMODEL
