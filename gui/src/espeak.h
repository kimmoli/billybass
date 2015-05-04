/*
    billybass, Billy Bass
*/

#ifndef ESPEAK_H
#define ESPEAK_H
#include <QObject>
#include <QSocketNotifier>
#include <QVariantList>
#include <QVariantMap>
#include "notificationmanager.h"

extern "C"
{
#include "speak_lib.h"
}

extern "C"
{
#include "iphbd/libiphb.h"
}


class Espeak : public QObject
{
    Q_OBJECT

public:
    explicit Espeak(QObject *parent = 0);
    ~Espeak();

    void replay();
    QVariantList getVoices();

    void requestSynth(QString message, QString language);

signals:
    void synthRequested();
    void synthComplete();
    void libespeakVersionChanged(QString ver);

public slots:
    void synth();

private:
    void init();
    void setLanguage(QString language = QString());
    void terminate();

    QString _libespeakVersion;
    bool _espeakInitialized;
    bool _terminating;
    QString _language;
    int _synthFlags;
    QString _stringToSynth;

};


#endif // ESPEAK_H

