#
# Project billybass, Billy Bass
#

TARGET = billybass

CONFIG += sailfishapp

include(../common.pri)

DEFINES += "APPVERSION=\\\"$${SPECVERSION}\\\""

message($${DEFINES})

SOURCES += \
    src/billybass.cpp \
    src/espeak.cpp
	
HEADERS += src/espeak.h

OTHER_FILES += \
    qml/billybass.qml \
    qml/cover/CoverPage.qml \
    qml/pages/Billybass.qml \
    qml/pages/AboutPage.qml \
    billybass.png \
    billybass.desktop

LIBS += -L$$OUT_PWD/../espeak/ -lespeak

DEPENDPATH += ../espeak/linux_32bit/shared_library
INCLUDEPATH += ../espeak/linux_32bit/shared_library

espeak_data.files = $$PWD/../espeak/espeak-data/*
espeak_data.path = $${DATADIR}

lib.files = $$OUT_PWD/../espeak/libespeak.so*
lib.path = /usr/share/billybass/lib

INSTALLS += lib espeak_data
