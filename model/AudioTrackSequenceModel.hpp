#ifndef MUSEC_MODEL_AUDIOTRACKSEQUENCEMODEL
#define MUSEC_MODEL_AUDIOTRACKSEQUENCEMODEL

#include "audio/arrangement/TrackSequence.hpp"
#include "model/ModelBase.hpp"

#include <QAbstractListModel>

namespace Musec
{
namespace Model
{
class AudioTrackSequenceModel: public QAbstractListModel
{
    Q_OBJECT
public:
    enum RoleNames
    {
        StartTimeRole = Qt::UserRole,
        EndTimeRole,
        ClipRole,
        ClipStartsAtRole,
        RoleNameCount
    };
public:
    explicit AudioTrackSequenceModel(QObject* parent = nullptr);
    virtual ~AudioTrackSequenceModel();
public:
    const Musec::Audio::Arrangement::AudioTrackSequence& getTrackSequence() const;
    Musec::Audio::Arrangement::AudioTrackSequence& getTrackSequence();
    void setTrackSequence(const Musec::Audio::Arrangement::AudioTrackSequence&);
    void setTrackSequence(Musec::Audio::Arrangement::AudioTrackSequence&&);
    int clipCount() const;
    static constexpr int columnSize();
public:
    virtual int rowCount(const QModelIndex&) const override;
    virtual int columnCount(const QModelIndex&) const override;
    virtual QVariant data(const QModelIndex&, int) const override;
protected:
    virtual RoleNamesType roleNames() const override;
private:
    Musec::Audio::Arrangement::AudioTrackSequence trackSequence_;
    RoleNamesType roleNames_;
};
}
}

#endif //MUSEC_MODEL_AUDIOTRACKSEQUENCEMODEL
