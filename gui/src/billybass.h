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
#include <contextproperty.h>
#include "espeak.h"
#include "languagedectector.h"

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
    Q_PROPERTY(bool boost READ readBoost WRITE writeBoost NOTIFY boostChanged())
    Q_PROPERTY(bool autolanguage READ readAutolanguage WRITE writeAutolanguage NOTIFY autolanguageChanged())

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
    Q_INVOKABLE void setVariant(QString variant);

    bool readStfu() { return _stfu; }
    bool readBoost() { return _boost; }
    bool readAutolanguage() { return _autolanguage; }

    void writeStfu(bool stfu);
    void writeBoost(bool boost);
    void writeAutolanguage(bool autolanguage);

signals:
    void versionChanged();
    void libespeakVersionChanged();
    void languageChanged();
    void stfuChanged();
    void boostChanged();
    void autolanguageChanged();

public slots:
    void espeakVersion(QString ver);
    void synthComplete();
    void changeLanguage(QString language);

private slots:
    void speakNotification(QString message);
    void heartbeatReceived(int sock);
    void iphbStop();
    void iphbStart(QString message);
    void threadFinished();
    void threadStarted();
    void propertyProfileNameChanged();

private:
    QString _libespeakVersion;
    bool _espeakInitialized;
    bool _terminating;
    QString _language;
    int _synthFlags;
    QString _lastStringSynth;
    QString _variant;
    QString _iphbMessage;
    bool _autolanguage;

    NotificationManager *notifications;

    iphb_t iphbdHandler;
    int iphb_fd;
    QSocketNotifier *iphbNotifier;
    bool iphbRunning;

    Espeak *espeak;
    QThread *thread;

    bool _stfu;
    bool _boost;
    bool _firstRun;

    QScopedPointer<ContextProperty> propertyProfileName;

    LanguageDetector *languagedetector;
};


#endif // BillyBass_H

