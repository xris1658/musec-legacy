#ifndef MUSEC_MODEL_AUTOMATIONMODEL
#define MUSEC_MODEL_AUTOMATIONMODEL

#include "audio/base/Automation.hpp"
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
    const Audio::Base::Automation& getAutomation() const;
    void setAutomation(const Musec::Audio::Base::Automation& automation);
    void setAutomation(Musec::Audio::Base::Automation&& automation);
public:
    Q_INVOKABLE int count() const noexcept;
    Q_INVOKABLE bool empty() const noexcept;
    static constexpr int columnSize() noexcept;
    Q_INVOKABLE double timeOfPoint(int index) const;
    Q_INVOKABLE double valueOfPoint(int index) const;
    Q_INVOKABLE double curveOfPoint(int index) const;
    Q_INVOKABLE double minValue() const noexcept;
    Q_INVOKABLE double maxValue() const noexcept;
    Q_INVOKABLE int pointCountAtTime(int time) const;
    Q_INVOKABLE int firstPointIndexAtTime(int time) const;
    Q_INVOKABLE double valueAtTime(int time) const;
    Q_INVOKABLE int insertPoint(int time, double value, double curve, int indexInEqualTimePoint);
    Q_INVOKABLE void deletePoint(int index);
    Q_INVOKABLE void setValueOfPoint(int index, double value);
    Q_INVOKABLE int setTimeOfPoint(int index, int time, int indexInEqualTimePoint);
    Q_INVOKABLE void setCurveOfPoint(int index, double curve);
    Q_INVOKABLE void clearPoints();
public:
    virtual int rowCount(const QModelIndex&) const override;
    virtual int columnCount(const QModelIndex&) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;
protected:
public:
    QHash<int, QByteArray> roleNames() const override;
private:
    Musec::Audio::Base::Automation automation_;
    RoleNamesType roleNames_;
};
}
}

#endif //MUSEC_MODEL_AUTOMATIONMODEL
