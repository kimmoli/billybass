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
    src/billybass.cpp \
    src/espeak.cpp \
    src/notificationmanager.cpp
	
HEADERS += src/espeak.h \
    src/notificationmanager.h

OTHER_FILES += \
    qml/billybass.qml \
    qml/cover/CoverPage.qml \
    qml/pages/Billybass.qml \
    qml/pages/AboutPage.qml \
    harbour-billybass.png \
    harbour-billybass.desktop \
    config\espeak.conf

LIBS += -L$$OUT_PWD/../espeak/ -lespeak

QMAKE_RPATHDIR += /usr/share/$${TARGET}/lib

DEPENDPATH += ../espeak/linux_32bit/shared_library
INCLUDEPATH += ../espeak/linux_32bit/shared_library

espeak_data.files = $$PWD/../espeak/espeak-data/*
espeak_data.path = $${DATADIR}

lib.files = $$OUT_PWD/../espeak/libespeak.so*
lib.path = /usr/share/$${TARGET}/lib

policy.files = $$PWD/config/espeak.conf
policy.path = /etc/pulse/xpolicy.conf.d

INSTALLS += lib espeak_data policy
