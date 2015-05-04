/*
    billybass, Billy Bass
*/

#ifndef BillyBass_H
#define BillyBass_H
#include <QObject>
#include <QSocketNotifier>
#include <QVariantList>
#include <QVariantMap>
#include "notificationmanager.h"
#include "espeak.h"

extern "C"
{
#include "iphbd/libiphb.h"
}

class BillyBass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString version READ readVersion NOTIFY versionChanged())
    Q_PROPERTY(QString libespeakVersion READ readLibespeakVersion NOTIFY libespeakVersionChanged())
    Q_PROPERTY(QString language READ readLanguage NOTIFY languageChanged())
    Q_PROPERTY(bool stfu READ readStfu WRITE writeStfu NOTIFY stfuChanged())

public:
    explicit BillyBass(QObject *parent = 0);
    ~BillyBass();

    QString readVersion();
    Q_INVOKABLE void synth(QString text);
    QString readLibespeakVersion() { return _libespeakVersion; }
    QString readLanguage() { return _language; }
    Q_INVOKABLE void setLanguage(QString language = QString());
    Q_INVOKABLE void replay();
    Q_INVOKABLE QVariantList getVoices();

    bool readStfu() { return _stfu; }
    void writeStfu(bool stfu);

signals:
    void versionChanged();
    void libespeakVersionChanged();
    void languageChanged();
    void stfuChanged();

public slots:
    void espeakVersion(QString ver);
    void synthComplete();

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

    iphb_t iphbdHandler;
    int iphb_fd;
    QSocketNotifier *iphbNotifier;
    bool iphbRunning;

    Espeak *espeak;
    QThread *thread;

    bool _stfu;
};


#endif // BillyBass_H

