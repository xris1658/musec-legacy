#include "CompleteTrack.hpp"

namespace Musec::Entities
{
CompleteTrack::CompleteTrack(QObject* parent, const QString& trackName, QColor trackColor,
    TrackType trackType, int height, double gain, double panning,
    bool trackMute, bool trackSolo, bool trackInvertPhase, bool trackArmRecording):
    QObject(parent),
    trackName_(trackName),
    trackColor_(trackColor),
    trackType_(trackType),
    height_(height),
    gain_(gain),
    panning_(panning),
    trackMute_(trackMute),
    trackSolo_(trackSolo),
    trackInvertPhase_(trackInvertPhase),
    trackArmRecording_(trackArmRecording)
{

}

const QString& CompleteTrack::getTrackName() const
{
    return trackName_;
}

void CompleteTrack::setTrackName(const QString& trackName)
{
    trackName_ = trackName;
}

const QColor& CompleteTrack::getTrackColor() const
{
    return trackColor_;
}

void CompleteTrack::setTrackColor(const QColor& trackColor)
{
    trackColor_ = trackColor;
}

CompleteTrack::TrackType CompleteTrack::getTrackType() const
{
    return trackType_;
}

void CompleteTrack::setTrackType(CompleteTrack::TrackType trackType)
{
    trackType_ = trackType;
}

int CompleteTrack::getHeight() const
{
    return height_;
}

void CompleteTrack::setHeight(int height)
{
    height_ = height;
}

double CompleteTrack::getGain() const
{
    return gain_;
}

void CompleteTrack::setGain(double gain)
{
    gain_ = gain;
}

double CompleteTrack::getPanning() const
{
    return panning_;
}

void CompleteTrack::setPanning(double panning)
{
    panning_ = panning;
}

bool CompleteTrack::isTrackMute() const
{
    return trackMute_;
}

void CompleteTrack::setTrackMute(bool trackMute)
{
    trackMute_ = trackMute;
}

bool CompleteTrack::isTrackSolo() const
{
    return trackSolo_;
}

void CompleteTrack::setTrackSolo(bool trackSolo)
{
    trackSolo_ = trackSolo;
}

bool CompleteTrack::isTrackInvertPhase() const
{
    return trackInvertPhase_;
}

void CompleteTrack::setTrackInvertPhase(bool trackInvertPhase)
{
    trackInvertPhase_ = trackInvertPhase;
}

bool CompleteTrack::isTrackArmRecording() const
{
    return trackArmRecording_;
}

void CompleteTrack::setTrackArmRecording(bool trackArmRecording)
{
    trackArmRecording_ = trackArmRecording;
}
}
