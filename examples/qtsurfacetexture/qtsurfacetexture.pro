TEMPLATE = app

QT += qml quick

QT += androidextras

SOURCES += main.cpp \
    qandroidmediaplayer.cpp \
    qsurfacetexture.cpp

HEADERS += \
    qandroidmediaplayer.h \
    qsurfacetexture.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/src/com/kdab/android/SurfaceTextureListener.java \
    android/AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
