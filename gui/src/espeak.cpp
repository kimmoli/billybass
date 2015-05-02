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
    _espeakInitialized = false;
    _language = "N/A";
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
    int synth_flags = espeakCHARS_AUTO | espeakPHONEMES | espeakENDPAUSE;

    if (text.isEmpty())
        text = "hiljaa";

    if (!_espeakInitialized)
    {
        const char *version;
        const char *path_data;
        int sampleRate;

        _language = QLocale::system().name().split('_').at(0);

        sampleRate = espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0);
        version = espeak_Info(&path_data);
        qDebug() << "samplerate:" << sampleRate;
        qDebug() << "version:" << QString(version);
        qDebug() << "data path:" << QString(path_data);

        _libespeakVersion = QString(version);
        emit libespeakVersionChanged();

        qDebug() << "setting language to" << _language;

        if (espeak_SetVoiceByName(_language.toLocal8Bit().data()) != EE_OK)
        {
            qDebug() << "language set failed, fallback to english";
            _language = "en";
            if (espeak_SetVoiceByName(_language.toLocal8Bit().data()) != EE_OK)
            {
                qDebug() << "language fallback failed, quitting here";
                return;
            }
        }
        emit languageChanged();

        _espeakInitialized = true;
    }

    espeak_ERROR ret = espeak_Synth(text.toLatin1().data(), text.length()+1, 0, POS_CHARACTER, 0, synth_flags, NULL, NULL);
    if (ret != EE_OK)
    {
        qDebug() << "synth failed" << ret;
    }
    qDebug() << "done";
}

