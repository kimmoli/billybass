/*
    billybass, Billy Bass
*/

#ifndef ESPEAK_H
#define ESPEAK_H
#include <QObject>
extern "C" {
#include "speak_lib.h"
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

signals:
    void versionChanged();
    void libespeakVersionChanged();
    void languageChanged();

private:
    QString _libespeakVersion;
    bool _espeakInitialized;
    QString _language;

};


#endif // ESPEAK_H

