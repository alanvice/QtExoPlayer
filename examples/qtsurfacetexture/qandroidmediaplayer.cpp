#include "qandroidmediaplayer.h"
#include "qsurfacetexture.h"

#include <QAndroidJniEnvironment>
#include <QtAndroid>

QAndroidMediaPlayer::QAndroidMediaPlayer(QObject *parent)
    : QObject(parent)
{
    QAndroidJniEnvironment env;
    QAndroidJniObject m_mediaBuilder(
                "com/google/android/exoplayer2/SimpleExoPlayer$Builder",
                "(Landroid/content/Context;)V",
                QtAndroid::androidContext().object<jobject>());

    //Di seguito implementazione di questa chiamata: SimpleExoPlayer.Builder(context).build();
    m_mediaPlayer = m_mediaBuilder.callObjectMethod("build","()Lcom/google/android/exoplayer2/SimpleExoPlayer;");
    if(m_mediaPlayer.isValid())
        qDebug() << "*****  exoPlayer OK  *****";
    else
        qDebug() << "*****  exoPlayer ERROR  *****";
}

QAndroidMediaPlayer::~QAndroidMediaPlayer()
{
    QAndroidJniEnvironment env;
    m_mediaPlayer.callMethod<void>("stop","(Z)V",true); // stop and reset
    m_mediaPlayer.callMethod<void>("release");
}

QSurfaceTexture *QAndroidMediaPlayer::videoOut() const
{
    return m_videoOut;
}

bool QAndroidMediaPlayer::isPlaying() const
{
    return m_isPlaying;
}

void QAndroidMediaPlayer::setVideoOut(QSurfaceTexture *videoOut)
{
    if (m_videoOut == videoOut)
        return;
    if (m_videoOut)
        m_videoOut->disconnect(this);
    m_videoOut = videoOut;

    auto setSurfaceTexture = [=]{
        // Create a new Surface object from our SurfaceTexture
        QAndroidJniObject surface("android/view/Surface",
                                  "(Landroid/graphics/SurfaceTexture;)V",
                                   m_videoOut->surfaceTexture().object());

        // Set the new surface to m_mediaPlayer object
        m_mediaPlayer.callMethod<void>("setVideoSurface",
                                       "(Landroid/view/Surface;)V",
                                       surface.object());
    };

    if (videoOut->surfaceTexture().isValid())
        setSurfaceTexture();
    else
        connect(m_videoOut.data(), &QSurfaceTexture::surfaceTextureChanged, this, setSurfaceTexture);
    emit videoOutChanged();
}

void QAndroidMediaPlayer::playFile(const QString &file)
{
    QAndroidJniEnvironment env;
    m_mediaPlayer.callMethod<void>("stop","(Z)V",true); // try to stop the media player and reset.

    QAndroidJniObject userAgent = QAndroidJniObject::callStaticObjectMethod(
                "com.google.android.exoplayer2.util.Util",
                "getUserAgent",
                "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;",
                QtAndroid::androidContext().object<jobject>(),
                QAndroidJniObject::fromString("com.kdab.android.examples.qsurfacetexture").object<jobject>());
    if(userAgent.isValid())
        qDebug() << "*****  userAgent OK  *****";
    else
        qDebug() << "*****  userAgent ERROR  *****";

    QAndroidJniObject dataSourceFactory("com/google/android/exoplayer2/upstream/DefaultDataSourceFactory",
                              "(Landroid/content/Context;Ljava/lang/String;)V",
                               QtAndroid::androidContext().object<jobject>(),
                               userAgent.object<jobject>());

    QAndroidJniObject localSource = QAndroidJniObject(
                "com/google/android/exoplayer2/source/ProgressiveMediaSource$Factory",
                "(Lcom/google/android/exoplayer2/upstream/DataSource$Factory;)V",
                dataSourceFactory.object<jobject>());
    if(localSource.isValid())
        qDebug() << "*****  localSource OK  *****";
    else
        qDebug() << "*****  localSource ERROR  *****";

    QAndroidJniObject uri = QAndroidJniObject::callStaticObjectMethod("android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;", QAndroidJniObject::fromString(file).object<jobject>());
    QAndroidJniObject videoSource = localSource.callObjectMethod(
                "createMediaSource",
                "(Landroid/net/Uri;)Lcom/google/android/exoplayer2/source/ProgressiveMediaSource;",
                uri.object());
    if(videoSource.isValid())
        qDebug() << "*****  videoSource OK  *****";
    else
        qDebug() << "*****  videoSource ERROR  *****";


    // prepare media player
    m_mediaPlayer.callMethod<void>("prepare","(Lcom/google/android/exoplayer2/source/MediaSource;)V",videoSource.object());

    // set repeat off:0, infinite this file 1, infinite playlist 2
    //m_mediaPlayer.callMethod<void>("setRepeatMode","(I)V",2);

    // start playing
    m_mediaPlayer.callMethod<void>("setPlayWhenReady","(Z)V",true);
}
