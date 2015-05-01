TEMPLATE = lib
QT =
TARGET = espeak
VERSION = 1.1.48

include(../common.pri)

DEFINES += "PATH_ESPEAK_DATA=\\\"$${DATADIR}\\\""
DEFINES += USE_PULSEAUDIO
DEFINES += USE_ASYNC
DEFINES += DEBUG_ENABLED

CONFIG += link_pkgconfig
PKGCONFIG += libpulse

message($${DEFINES})

INCLUDEPATH += src

QMAKE_POST_LINK += $$PWD/libcopy.sh $$OUT_PWD lib$${TARGET} $${VERSION}

HEADERS += src/debug.h \
           src/espeak_command.h \
           src/event.h \
           src/fifo.h \
           src/klatt.h \
           src/mbrowrap.h \
           src/phoneme.h \
           src/portaudio.h \
           src/portaudio18.h \
           src/portaudio19.h \
           src/sintab.h \
           src/sonic.h \
           src/speak_lib.h \
           src/speech.h \
           src/StdAfx.h \
           src/synthesize.h \
           src/translate.h \
           src/voice.h \
           src/wave.h

SOURCES += src/speak_lib.cpp \
           src/compiledict.cpp \
           src/dictionary.cpp \
           src/intonation.cpp \
           src/readclause.cpp \
           src/setlengths.cpp \
           src/numbers.cpp \
           src/synth_mbrola.cpp \
           src/synthdata.cpp \
           src/synthesize.cpp \
           src/translate.cpp \
           src/mbrowrap.cpp \
           src/tr_languages.cpp \
           src/voices.cpp \
           src/wavegen.cpp \
           src/phonemelist.cpp \
           src/espeak_command.cpp \
           src/event.cpp \
           src/fifo.cpp \
           src/wave_pulse.cpp \
           src/debug.cpp \
           src/klatt.cpp \
           src/sonic.cpp

OTHER_FILES += \
    espeak-data/af_dict \
    espeak-data/am_dict \
    espeak-data/an_dict \
    espeak-data/as_dict \
    espeak-data/az_dict \
    espeak-data/bg_dict \
    espeak-data/bn_dict \
    espeak-data/ca_dict \
    espeak-data/cs_dict \
    espeak-data/cy_dict \
    espeak-data/da_dict \
    espeak-data/de_dict \
    espeak-data/el_dict \
    espeak-data/en_dict \
    espeak-data/eo_dict \
    espeak-data/es_dict \
    espeak-data/et_dict \
    espeak-data/eu_dict \
    espeak-data/fa_dict \
    espeak-data/fi_dict \
    espeak-data/fr_dict \
    espeak-data/ga_dict \
    espeak-data/gd_dict \
    espeak-data/grc_dict \
    espeak-data/gu_dict \
    espeak-data/hbs_dict \
    espeak-data/hi_dict \
    espeak-data/hu_dict \
    espeak-data/hy_dict \
    espeak-data/id_dict \
    espeak-data/intonations \
    espeak-data/is_dict \
    espeak-data/it_dict \
    espeak-data/jbo_dict \
    espeak-data/ka_dict \
    espeak-data/kl_dict \
    espeak-data/kn_dict \
    espeak-data/ko_dict \
    espeak-data/ku_dict \
    espeak-data/la_dict \
    espeak-data/lfn_dict \
    espeak-data/lt_dict \
    espeak-data/lv_dict \
    espeak-data/mk_dict \
    espeak-data/ml_dict \
    espeak-data/ms_dict \
    espeak-data/nci_dict \
    espeak-data/ne_dict \
    espeak-data/nl_dict \
    espeak-data/no_dict \
    espeak-data/or_dict \
    espeak-data/pa_dict \
    espeak-data/pap_dict \
    espeak-data/phondata \
    espeak-data/phonindex \
    espeak-data/phontab \
    espeak-data/pl_dict \
    espeak-data/pt_dict \
    espeak-data/ro_dict \
    espeak-data/ru_dict \
    espeak-data/si_dict \
    espeak-data/sk_dict \
    espeak-data/sl_dict \
    espeak-data/sq_dict \
    espeak-data/sv_dict \
    espeak-data/sw_dict \
    espeak-data/ta_dict \
    espeak-data/te_dict \
    espeak-data/tr_dict \
    espeak-data/ur_dict \
    espeak-data/vi_dict \
    espeak-data/zh_dict \
    espeak-data/zhy_dict \
    espeak-data/phondata-manifest \
    libcopy.sh

