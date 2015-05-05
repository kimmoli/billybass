/*
    billybass, Billy Bass
*/

#include "billybass.h"
#include <QDebug>
#include <QThread>

BillyBass::BillyBass(QObject *parent) :
    QObject(parent)
{
    emit versionChanged();
    _libespeakVersion = "N/A";
    _espeakInitialized = false;
    _language = QString();
    _synthFlags = espeakCHARS_AUTO | espeakPHONEMES | espeakENDPAUSE;
    _lastStringSynth = QString();
    _stfu = false;
    _firstRun = true;

    notifications = new NotificationManager();

    connect(notifications, SIGNAL(gotNotification(QString)), this, SLOT(speakNotification(QString)));

    iphbRunning = false;
    iphbdHandler = iphb_open(0);

    if (!iphbdHandler)
        qDebug() << "Error opening iphb";

    iphb_fd = iphb_get_fd(iphbdHandler);

    iphbNotifier = new QSocketNotifier(iphb_fd, QSocketNotifier::Read);

    if (!QObject::connect(iphbNotifier, SIGNAL(activated(int)), this, SLOT(heartbeatReceived(int))))
    {
        delete iphbNotifier, iphbNotifier = 0;
        qDebug() << "failed to connect iphbNotifier";
    }
    else
    {
        iphbNotifier->setEnabled(false);
    }

    if (iphbNotifier)
        qDebug() << "iphb initialized succesfully";

    thread = new QThread();
    espeak = new Espeak();

    espeak->moveToThread(thread);

    connect(espeak, SIGNAL(synthRequested()), thread, SLOT(start()));
    connect(thread, SIGNAL(started()), espeak, SLOT(synth()));
    connect(espeak, SIGNAL(synthComplete()), this, SLOT(synthComplete()));
    connect(espeak, SIGNAL(libespeakVersionChanged(QString)), this, SLOT(espeakVersion(QString)));

    /* Debugs */
    connect(thread, SIGNAL(started()), this, SLOT(threadStarted()));
    connect(thread, SIGNAL(finished()), this, SLOT(threadFinished()));

    propertyProfileName.reset(new ContextProperty("Profile.Name", this));
    QObject::connect(propertyProfileName.data(), SIGNAL(valueChanged()), this, SLOT(propertyProfileNameChanged()));
}

BillyBass::~BillyBass()
{
    if(iphbdHandler)
        (void)iphb_close(iphbdHandler);

    if(iphbNotifier)
        delete iphbNotifier;
}

QString BillyBass::readVersion()
{
    return APPVERSION;
}

void BillyBass::espeakVersion(QString ver)
{
    _libespeakVersion = ver;
    emit libespeakVersionChanged();
}

void BillyBass::propertyProfileNameChanged()
{
    qDebug() << "property Profile.Name changed" << propertyProfileName->value().toString();

    writeStfu(propertyProfileName->value().toString() == "silent");
}

void BillyBass::threadFinished()
{
    qDebug() << "thread finished";

    _firstRun = false;

    if (!espeak->isQueueEmpty())
    {
        qDebug() << "queue not empty, requesting to continue by adding empty string to queue";
        espeak->requestSynth(QString(), _language);
    }
}

void BillyBass::threadStarted()
{
    qDebug() << "thread started";
}

void BillyBass::synthComplete()
{
    qDebug() << "synth complete";

    thread->quit();
}

/* IPHB stuff */

void BillyBass::heartbeatReceived(int sock)
{
    Q_UNUSED(sock);

    qDebug() << "iphb heartbeat triggering synth" << _lastStringSynth;

    espeak->requestSynth(_lastStringSynth, _language);

    iphbStop();
}

void BillyBass::iphbStart()
{
    if (iphbRunning)
        return;

    qDebug() << "iphb start";

    if (!(iphbdHandler && iphbNotifier))
    {
        qCritical() << "iphbStart iphbHandler not ok";
        return;
    }

    time_t unixTime;

    unixTime = iphb_wait(iphbdHandler, 3, 4 , 0);

    if (unixTime == (time_t)-1)
    {
        qCritical() << "iphbStart timer failed";
        return;
    }

    iphbNotifier->setEnabled(true);
    iphbRunning = true;
}

void BillyBass::iphbStop()
{
    if (!iphbRunning)
        return;

    qDebug() << "iphb stop";

    if (!(iphbdHandler && iphbNotifier))
    {
        qCritical() << "iphbStop iphbHandler not ok";
        return;
    }

    iphbNotifier->setEnabled(false);

    (void)iphb_discard_wakeups(iphbdHandler);

    iphbRunning = false;
}

/* STFU */

void BillyBass::writeStfu(bool stfu)
{
    _stfu = stfu;
    qDebug() << "stfu" << stfu;

    if (_stfu && thread->isRunning() && !_firstRun)
    {
        qDebug() << "cancelling current synth";
        espeak_Cancel();
        //espeak->terminate(true);
    }

    emit stfuChanged();
}

/* ESPEAK stuff */

void BillyBass::synth(QString text)
{
    qDebug() << "requesting synth" << text;

    espeak->requestSynth(text, _language);
    _lastStringSynth = text;
}

void BillyBass::setLanguage(QString language)
{
    qDebug() << "setting language" << language;

    _language = language;
    emit languageChanged();
}

void BillyBass::speakNotification(QString message)
{
    _lastStringSynth = message;

    qDebug() << "notification" << message;

    if (_stfu)
    {
        qDebug() << "stfu mode enabled, aborting";
        return;
    }

    iphbStart();

//    struct timespec shorttime;
//    shorttime.tv_sec = 3;
//    shorttime.tv_nsec = 0;

//    clock_nanosleep(CLOCK_MONOTONIC, 0, &shorttime, NULL);
}

void BillyBass::replay()
{
    qDebug() << "replay" << _lastStringSynth;

    espeak->requestSynth(_lastStringSynth, _language);
}

QVariantList BillyBass::getVoices()
{
    QVariantList tmp;
    QVariantMap map;

    /* TODO: Cleanup */

    int ix;
    const char *p;
    int len;
    int count;
    int c;
    unsigned int j;
    const espeak_VOICE *v;
    const char *lang_name;
    char age_buf[12];
    char buf[80];
    char f_out[180];
    const espeak_VOICE **voices;

    static char genders[4] = {'-','M','F','-'};

    voices = espeak_ListVoices(NULL);

    qDebug() << "Pty Language Age/Gender VoiceName          File          Other Languages";

    for(ix=0; (v = voices[ix]) != NULL; ix++)
    {
        count = 0;
        p = v->languages;
        while(*p != 0)
        {
            len = strlen(p+1);
            lang_name = p+1;

            if(v->age == 0)
                strcpy(age_buf,"   ");
            else
                sprintf(age_buf,"%3d",v->age);

            if(count==0)
            {
                for(j=0; j < sizeof(buf); j++)
                {
                    // replace spaces in the name
                    if((c = v->name[j]) == ' ')
                        c = '_';
                    if((buf[j] = c) == 0)
                        break;
                }
                sprintf(f_out,"%2d  %-12s%s%c  %-20s %-13s ",
               p[0],lang_name,age_buf,genders[v->gender],buf,v->identifier);

                map.clear();
                map.insert("language", QString(lang_name));
                map.insert("voicename", QString(buf));
                tmp.append(map);
            }
            else
            {
                sprintf(f_out + strlen(f_out),"(%s %d)",lang_name,p[0]);
            }
            count++;
            p += len+2;
        }
        qDebug() << QString(f_out);
    }
    return tmp;
}
