#ifndef MUSEC_MODEL_TEMPOAUTOMATIONMODEL
#define MUSEC_MODEL_TEMPOAUTOMATIONMODEL

#include "audio/base/TempoAutomation.hpp"
#include "model/ModelBase.hpp"

#include <QAbstractListModel>

namespace Musec
{
namespace Model
{
class TempoAutomationModel: public QAbstractListModel
{
    Q_OBJECT
public:
    using TempoAutomation = Musec::Audio::Base::TempoAutomation<96>;
public:
    enum RoleNames
    {
        TimeRole = Qt::UserRole,
        ValueRole,
        CurveRole,
        RoleEnd
    };
public:
    explicit TempoAutomationModel(QObject* parent = nullptr);
    ~TempoAutomationModel();
public:
    const TempoAutomation& getTempoAutomation() const;
    void setTempoAutomation(const TempoAutomation& automation);
    void setTempoAutomation(TempoAutomation&& automation);
public:
    Q_INVOKABLE int count() const noexcept;
    Q_INVOKABLE bool empty() const noexcept;
    static constexpr int columnSize() noexcept;
    Q_INVOKABLE int timeOfPoint(int index) const;
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
    RoleNamesType roleNames() const override;
private:
    void automationSet(int oldRowCount, int newRowCount);
private:
    TempoAutomation automation_;
};
}
}

#endif //MUSEC_MODEL_TEMPOAUTOMATIONMODEL
