/*
    billybass, Billy Bass
*/

#include "espeak.h"
#include <QDebug>
#include <QThread>

Espeak::Espeak(QObject *parent) :
    QObject(parent)
{
    emit versionChanged();
    _libespeakVersion = "N/A";
    _espeakInitialized = false;
    _language = "N/A";
    _synthFlags = espeakCHARS_AUTO | espeakPHONEMES | espeakENDPAUSE;
    _lastStringSynth = QString();

    notifications = new NotificationManager();

    connect(notifications, SIGNAL(gotNotification(QString)), this, SLOT(speakNotification(QString)));
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
    if (!_espeakInitialized)
        return;

    if (text.isEmpty())
    {
        qDebug() << "not synthesizing empty string";
        return;
    }

    qDebug() << "synth:" << text;

    espeak_ERROR ret = espeak_Synth(text.toLatin1().data(), text.length()+1, 0, POS_CHARACTER, 0, _synthFlags, NULL, NULL);
    if (ret != EE_OK)
    {
        qDebug() << "synth failed" << ret;
    }

    _lastStringSynth = text;
}

void Espeak::init()
{
    const char *version;
    const char *path_data;
    int sampleRate;

    sampleRate = espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0);
    version = espeak_Info(&path_data);
    qDebug() << "samplerate:" << sampleRate;
    qDebug() << "version:" << QString(version);
    qDebug() << "data path:" << QString(path_data);

    _libespeakVersion = QString(version);
    emit libespeakVersionChanged();

    setLanguage(); // Select default language by locale

    _espeakInitialized = true;
}

void Espeak::setLanguage(QString language)
{
    QString lang = language;

    if (lang.isEmpty())
        lang = QLocale::system().name().split('_').at(0);

    qDebug() << "setting language to" << lang;

    if (espeak_SetVoiceByName(lang.toLocal8Bit().data()) != EE_OK)
    {
        qDebug() << "language set failed, fallback to english";
        lang= "en";
        if (espeak_SetVoiceByName(lang.toLocal8Bit().data()) != EE_OK)
        {
            qDebug() << "language fallback failed.";
            return;
        }
    }

    _language = lang;
    emit languageChanged();
}

void Espeak::speakNotification(QString message)
{
    if (!_espeakInitialized)
        return;

    /* Allow system notification sound to be played */
    QThread::msleep(2000);

    synth(message);
}

void Espeak::replay()
{
    synth(_lastStringSynth);
}
