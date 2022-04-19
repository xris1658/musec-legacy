#ifndef MUSEC_AUDIO_ENGINE_COMPLETETRACK
#define MUSEC_AUDIO_ENGINE_COMPLETETRACK

#include "audio/track/ITrack.hpp"

#include <QtCore/qobjectdefs.h>
#include <QColor>
#include <QObject>
#include <qqml.h>

namespace Musec
{
namespace Entities
{
class CompleteTrack: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString trackName_ READ getTrackName WRITE setTrackName NOTIFY trackNameChanged)
    Q_PROPERTY(QColor trackColor_ READ getTrackColor WRITE setTrackColor NOTIFY trackColorChanged)
    Q_PROPERTY(TrackType trackType_ READ getTrackType WRITE setTrackType NOTIFY trackTypeChanged)
    Q_PROPERTY(int height_ READ getHeight WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(double gain_ READ getGain WRITE setGain NOTIFY gainChanged)
    Q_PROPERTY(double panning_ READ getPanning WRITE setPanning NOTIFY panningChanged)
    Q_PROPERTY(bool trackMute_ READ isTrackMute WRITE setTrackMute NOTIFY trackMuteChanged)
    Q_PROPERTY(bool trackSolo_ READ isTrackSolo WRITE setTrackSolo NOTIFY trackSoloChanged)
    Q_PROPERTY(bool trackInvertPhase_ READ isTrackInvertPhase WRITE setTrackInvertPhase NOTIFY trackInvertPhaseChanged)
    Q_PROPERTY(bool trackArmRecording_ READ isTrackArmRecording WRITE setTrackArmRecording NOTIFY trackArmRecordingChanged)
    QML_ELEMENT
public:
    enum TrackType
    {
        AudioTrack,
        MIDITrack,
        InstrumentTrack
    };
    Q_ENUM(TrackType)
public:
    CompleteTrack(QObject* parent = nullptr,
        const QString& trackName = "", QColor trackColor = QColor(0, 0, 0),
        TrackType trackType = TrackType::AudioTrack,
        int height = 60, double gain = 1.0, double panning = 0.0,
        bool trackMute = false, bool trackSolo = false,
        bool trackInvertPhase = false, bool trackArmRecording = false);
public:
    const QString& getTrackName() const;
    void setTrackName(const QString& trackName);
    const QColor& getTrackColor() const;
    void setTrackColor(const QColor& trackColor);
    TrackType getTrackType() const;
    void setTrackType(TrackType trackType);
    int getHeight() const;
    void setHeight(int height);
    double getGain() const;
    void setGain(double gain);
    double getPanning() const;
    void setPanning(double panning);
    bool isTrackMute() const;
    void setTrackMute(bool trackMute);
    bool isTrackSolo() const;
    void setTrackSolo(bool trackSolo);
    bool isTrackInvertPhase() const;
    void setTrackInvertPhase(bool trackInvertPhase);
    bool isTrackArmRecording() const;
    void setTrackArmRecording(bool trackArmRecording);
signals:
    void trackNameChanged();
    void trackColorChanged();
    void trackTypeChanged();
    void heightChanged();
    void gainChanged();
    void panningChanged();
    void trackMuteChanged();
    void trackSoloChanged();
    void trackInvertPhaseChanged();
    void trackArmRecordingChanged();
private:
    QString trackName_;
    QColor trackColor_;
    TrackType trackType_;
    int height_;
    double gain_;
    double panning_;
    bool trackMute_;
    bool trackSolo_;
    bool trackInvertPhase_;
    bool trackArmRecording_;
};
}
}

#endif //MUSEC_AUDIO_ENGINE_COMPLETETRACK
