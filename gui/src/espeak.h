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

public:
    explicit Espeak(QObject *parent = 0);
    ~Espeak();

    QString readVersion();
    //Q_INVOKABLE void setLanguage(QString lang);
    Q_INVOKABLE QString libVersion();
    Q_INVOKABLE void synth(QString text);

signals:
    void versionChanged();

    //private:

};


#endif // ESPEAK_H

