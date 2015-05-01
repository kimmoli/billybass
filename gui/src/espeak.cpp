/*
    billybass, Billy Bass
*/

#include "espeak.h"
#include <QDebug>

Espeak::Espeak(QObject *parent) :
    QObject(parent)
{
    emit versionChanged();
}

Espeak::~Espeak()
{
}

QString Espeak::readVersion()
{
    return APPVERSION;
}

QString Espeak::libVersion()
{
    const char *version;
    const char *path_data;
    int sampleRate = espeak_Initialize(AUDIO_OUTPUT_SYNCH_PLAYBACK, 0, NULL, espeakINITIALIZE_DONT_EXIT);
    version = espeak_Info(&path_data);
    qDebug() << "eSpeak text-to-speech:" << QString(version);
    qDebug() << "data location:" << QString(path_data);
    qDebug() << "samplerate:" << sampleRate;

    return QString(version);
}

void Espeak::synth(QString text)
{
    if (text.isEmpty())
        text = "hiljaa";

    int synth_flags = espeakCHARS_AUTO | espeakPHONEMES | espeakENDPAUSE;

    int sampleRate = espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0);
    qDebug() << "samplerate:" << sampleRate;

    if (espeak_SetVoiceByName("fi") != EE_OK)
    {
        qDebug() << "language set failed";
        return;
    }

    espeak_ERROR ret = espeak_Synth(text.toLatin1().data(), text.length()+1, 0, POS_CHARACTER, 0, synth_flags, NULL, NULL);
    if (ret != EE_OK)
    {
        qDebug() << "synth failed" << ret;
    }
}

