#
# Project billybass, Billy Bass
#

TARGET = harbour-billybass

CONFIG += sailfishapp
QT += dbus

include(../common.pri)

DEFINES += "APPVERSION=\\\"$${SPECVERSION}\\\""

message($${DEFINES})

SOURCES += \
    src/espeak.cpp \
    src/notificationmanager.cpp \
    src/main.cpp \
    src/billybass.cpp

HEADERS += \
    src/espeak.h \
    src/notificationmanager.h \
    src/billybass.h

OTHER_FILES += \
    qml/billybass.qml \
    qml/cover/CoverPage.qml \
    qml/pages/Billybass.qml \
    qml/pages/AboutPage.qml \
    harbour-billybass.png \
    harbour-billybass.desktop \
    config\espeak.conf \
    qml/pages/LanguageSelector.qml

# iphb stuff

INCLUDEPATH += \
    ./3rdparty/libdsme/include/ \
    ./3rdparty/mce-dev/include/ \
    ./3rdparty/libiphb/src/

LIBS += -lrt

SOURCES += \
    3rdparty/libiphb/src/libiphb.c

HEADERS += \
    3rdparty/libiphb/src/libiphb.h \
    3rdparty/libiphb/src/iphb_internal.h \
    3rdparty/libdsme/include/dsme/messages.h \
    3rdparty/mce-dev/include/mce/dbus-names.h

# end of iphb stuff

# espeak stuff

LIBS += -L$$OUT_PWD/../espeak/ -lespeak

QMAKE_RPATHDIR += /usr/share/$${TARGET}/lib

DEPENDPATH += ../espeak/linux_32bit/shared_library
INCLUDEPATH += ../espeak/linux_32bit/shared_library

espeak_data.files = $$PWD/../espeak/espeak-data/*
espeak_data.path = $${DATADIR}

lib.files += \
    $$OUT_PWD/../espeak/libespeak.so \
    $$OUT_PWD/../espeak/libespeak.so.1 \
    $$OUT_PWD/../espeak/libespeak.so.1.1 \
    $$OUT_PWD/../espeak/libespeak.so.1.1.48
lib.path = /usr/share/$${TARGET}/lib

policy.files = $$PWD/config/espeak.conf
policy.path = /etc/pulse/xpolicy.conf.d

INSTALLS += lib espeak_data policy

# end of espeak sutff
