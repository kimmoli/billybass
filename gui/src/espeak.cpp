/*
    billybass, Billy Bass
*/

#include "espeak.h"
#include <QDebug>
#include <QThread>

Espeak::Espeak(QObject *parent) :
    QObject(parent)
{
    _libespeakVersion = QString();
    _stringToSynth.clear();
    _espeakInitialized = false;
    _language = QString();
    _synthFlags = espeakCHARS_AUTO | espeakPHONEMES | espeakENDPAUSE;
}

Espeak::~Espeak()
{
    terminate();
}

void Espeak::requestSynth(QString message, QString language)
{
    _stringToSynth.enqueue(message);
    _language = language;

    emit synthRequested();
}

void Espeak::synth()
{
    if (!_espeakInitialized)
        init();

    while (!_stringToSynth.isEmpty())
    {
        QString stringToSynth = _stringToSynth.dequeue();

        if (stringToSynth.isEmpty())
        {
            qDebug() << "not synthesizing empty string";
            terminate();

            emit synthComplete();

            return;
        }

        qDebug() << "synth:" << stringToSynth;

        espeak_ERROR ret = espeak_Synth(stringToSynth.toLatin1().data(), stringToSynth.length()+1, 0, POS_CHARACTER, 0, _synthFlags, NULL, NULL);
        if (ret != EE_OK)
        {
            qDebug() << "synth failed" << ret;
        }
    }

    terminate();

    emit synthComplete();
}

void Espeak::init()
{
    const char *version;
    const char *path_data;
    int sampleRate;

    if (_espeakInitialized)
        terminate();

    qDebug() << "initialising...";

    sampleRate = espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0);

    if (_libespeakVersion.isEmpty())
    {
        version = espeak_Info(&path_data);
        qDebug() << "samplerate:" << sampleRate;
        qDebug() << "version:" << QString(version);
        qDebug() << "data path:" << QString(path_data);

        _libespeakVersion = QString(version);
        emit libespeakVersionChanged(_libespeakVersion);
    }

    _espeakInitialized = true;

    setLanguage(_language);
}

void Espeak::setLanguage(QString language)
{
    QString lang = language;

    if (lang.isEmpty())
        lang = QLocale::system().name().split('_').at(0);

    qDebug() << "setting language to" << lang;

    if(_espeakInitialized)
    {
        if (espeak_SetVoiceByName(lang.toLocal8Bit().data()) != EE_OK)
        {
            qDebug() << "language set failed, fallback to english";
            lang= "english";
            if (espeak_SetVoiceByName(lang.toLocal8Bit().data()) != EE_OK)
            {
                qDebug() << "language fallback failed.";
                return;
            }
        }
    }
}

void Espeak::terminate(bool force)
{
    if (_espeakInitialized)
    {
        while (espeak_IsPlaying() && !force)
        {
            QThread::msleep(100);
        }

        if (force)
        {
            espeak_Cancel();
            QThread::msleep(100);
        }

        espeak_Terminate();

        QThread::msleep(100);

        _espeakInitialized = false;

        qDebug() << "terminated" << (force ? "(forced)" : "");

        if (force)
            emit synthComplete();
    }
}

