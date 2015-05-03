/*
    billybass, Billy Bass
*/

#ifndef ESPEAK_H
#define ESPEAK_H
#include <QObject>
#include <QSocketNotifier>
#include <QMutex>
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
    Q_PROPERTY(QString version READ readVersion NOTIFY versionChanged())
    Q_PROPERTY(QString libespeakVersion READ readLibespeakVersion NOTIFY libespeakVersionChanged())
    Q_PROPERTY(QString language READ readLanguage NOTIFY languageChanged())

public:
    explicit Espeak(QObject *parent = 0);
    ~Espeak();

    QString readVersion();
    Q_INVOKABLE void synth(QString text);
    QString readLibespeakVersion() { return _libespeakVersion; }
    QString readLanguage() { return _language; }
    Q_INVOKABLE void init();
    Q_INVOKABLE void setLanguage(QString language = QString());
    Q_INVOKABLE void replay();

signals:
    void versionChanged();
    void libespeakVersionChanged();
    void languageChanged();

private slots:
    void speakNotification(QString message);
    void heartbeatReceived(int sock);
    void iphbStop();
    void iphbStart();

private:
    QString _libespeakVersion;
    bool _espeakInitialized;
    bool _terminating;
    QString _language;
    int _synthFlags;
    QString _lastStringSynth;

    NotificationManager *notifications;

    void terminate();

    iphb_t iphbdHandler;
    int iphb_fd;
    QSocketNotifier *iphbNotifier;
    bool iphbRunning;

    QMutex mutex;
};


#endif // ESPEAK_H

