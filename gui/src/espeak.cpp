/*
    billybass, Billy Bass
*/

#include "espeak.h"
#include <QDebug>

Espeak::Espeak(QObject *parent) :
    QObject(parent)
{
    emit versionChanged();
    _libespeakVersion = "N/A";
}

Espeak::~Espeak()
{
}

QString Espeak::readVersion()
{
    return APPVERSION;
}


void Espeak::synth(QString text)
{
    const char *version;
    const char *path_data;

    if (text.isEmpty())
        text = "hiljaa";

    QString language = QLocale::system().name().split('_').at(0);

    int synth_flags = espeakCHARS_AUTO | espeakPHONEMES | espeakENDPAUSE;

    int sampleRate = espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0);
    version = espeak_Info(&path_data);
    qDebug() << "samplerate:" << sampleRate;
    qDebug() << "version:" << QString(version);
    qDebug() << "data path:" << QString(path_data);

    _libespeakVersion = QString(version);
    emit libespeakVersionChanged();

    qDebug() << "setting language to" << language;

    if (espeak_SetVoiceByName(language.toLocal8Bit().data()) != EE_OK)
    {
        qDebug() << "language set failed, fallback to english";
        if (espeak_SetVoiceByName("en") != EE_OK)
        {
            qDebug() << "language fallback failed, quitting here";
            return;
        }
    }

    espeak_ERROR ret = espeak_Synth(text.toLatin1().data(), text.length()+1, 0, POS_CHARACTER, 0, synth_flags, NULL, NULL);
    if (ret != EE_OK)
    {
        qDebug() << "synth failed" << ret;
    }
    qDebug() << "done";
}

