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

public:
    explicit Espeak(QObject *parent = 0);
    ~Espeak();

    QString readVersion();
    Q_INVOKABLE void synth(QString text);
    QString readLibespeakVersion() { return _libespeakVersion; }

signals:
    void versionChanged();
    void libespeakVersionChanged();

private:
    QString _libespeakVersion;
    bool _espeakInitialized;

};


#endif // ESPEAK_H

