#ifndef MUSEC_MODEL_VOLUMEFADERSCALEMODEL
#define MUSEC_MODEL_VOLUMEFADERSCALEMODEL

#include "model/ModelBase.hpp"

#include <QAbstractListModel>

#include <vector>

namespace Musec
{
namespace Model
{
class VolumeFaderScaleModel: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(double maxDecibel MEMBER maxDecibel_ READ maxDecibel WRITE setMaxDecibel NOTIFY maxDecibelChanged)
    Q_PROPERTY(double minDecibel MEMBER minDecibel_ READ minDecibel WRITE setMinDecibel NOTIFY minDecibelChanged)
public:
    enum RoleNames
    {
        PositionRole = Qt::UserRole,
        DecibelRole,
        RoleEnd
    };
public:
    VolumeFaderScaleModel(QObject* parent = nullptr);
public:
    double maxDecibel() const;
    bool setMaxDecibel(double maxDecibel);
    double minDecibel() const;
    bool setMinDecibel(double minDecibel);
public:
    int rowCount() const;
    static constexpr int columnSize() noexcept;
public:
    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;
public:
    Q_INVOKABLE bool insertPoint(double position, double decibel);
    Q_INVOKABLE bool removePoint(int index);
    Q_INVOKABLE double decibelFromPosition(double position);
    Q_INVOKABLE double positionFromDecibel(double decibel);
protected:
    RoleNamesType roleNames() const override;
signals:
    void maxDecibelChanged();
    void minDecibelChanged();
private:
    double maxDecibel_;
    double minDecibel_;
    std::vector<std::pair<double, double>> container_;

};
}
}

#endif //MUSEC_MODEL_VOLUMEFADERSCALEMODEL
