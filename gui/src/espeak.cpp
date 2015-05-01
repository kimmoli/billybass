/*
    billybass, Billy Bass
*/

#include "espeak.h"
#include "speak_lib.h"
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

void Espeak::test()
{
    const char *version;
    const char *path_data;
    espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS, 0, NULL, espeakINITIALIZE_DONT_EXIT);
    version = espeak_Info(&path_data);
    qDebug() << "eSpeak text-to-speech:" << QString(version);
    qDebug() << "data location:" << QString(path_data);
}
