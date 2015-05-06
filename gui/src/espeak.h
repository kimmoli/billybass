/*
    billybass, Billy Bass
*/

#ifndef ESPEAK_H
#define ESPEAK_H
#include <QObject>
#include <QSocketNotifier>
#include <QVariantList>
#include <QVariantMap>
#include <QQueue>
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
    bool isQueueEmpty() { return _stringToSynth.isEmpty(); }
    void terminate(bool force = false);

signals:
    void synthRequested();
    void synthComplete();
    void libespeakVersionChanged(QString ver);
    void languageChanged(QString language);

public slots:
    void synth();

private:
    void init();
    void setLanguage(QString language = QString());

    QString _libespeakVersion;
    bool _espeakInitialized;
    bool _terminating;
    QString _language;
    int _synthFlags;
    QQueue<QString> _stringToSynth;

};


#endif // ESPEAK_H

