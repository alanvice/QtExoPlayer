#ifndef QANDROIDMEDIAPLAYER_H
#define QANDROIDMEDIAPLAYER_H

#include <QAndroidJniObject>
#include <QObject>
#include <QPointer>

class QSurfaceTexture;
class QAndroidMediaPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSurfaceTexture *videoOut READ videoOut WRITE setVideoOut NOTIFY videoOutChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)

public:
    QAndroidMediaPlayer(QObject *parent = nullptr);
    ~QAndroidMediaPlayer();

    QSurfaceTexture *videoOut() const;
    void setVideoOut(QSurfaceTexture *videoOut);

    Q_INVOKABLE void playFile(const QString &file);
    bool isPlaying() const;

signals:
    void videoOutChanged();
    void playbackEnded();
    void isPlayingChanged();

private:
    QPointer<QSurfaceTexture> m_videoOut;
    QAndroidJniObject m_mediaPlayer;
    bool m_isPlaying = false;
};

#endif // QANDROIDMEDIAPLAYER_H
