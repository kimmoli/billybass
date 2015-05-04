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

void BillyBass::synthComplete()
{
    thread->quit();

    QThread::msleep(100);

    if (!espeak->isQueueEmpty())
        espeak->requestSynth(QString(), _language);
}

/* IPHB stuff */

void BillyBass::heartbeatReceived(int sock)
{
    Q_UNUSED(sock);

    qDebug() << "iphb heartbeat";

    iphbStop();

    if (_espeakInitialized)
        iphbStart();
}

void BillyBass::iphbStart()
{
    if (iphbRunning)
        return;

    qDebug() << "iphb start";

    if (!(iphbdHandler && iphbNotifier))
    {
        qDebug() << "iphbStart iphbHandler not ok";
        return;
    }

    time_t unixTime;

    unixTime = iphb_wait(iphbdHandler, 2, 5 , 0);

    if (unixTime == (time_t)-1)
    {
        qDebug() << "iphbStart timer failed";
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
        printf("iphbStop iphbHandler not ok\n");
        return;
    }

    iphbNotifier->setEnabled(false);

    (void)iphb_discard_wakeups(iphbdHandler);

    iphbRunning = false;
}

/* ESPEAK stuff */

void BillyBass::synth(QString text)
{
    espeak->requestSynth(text, _language);
    _lastStringSynth = text;
}

void BillyBass::setLanguage(QString language)
{
    _language = language;
    emit languageChanged();
}

void BillyBass::speakNotification(QString message)
{
    /* Allow system notification sound to be played */
    iphbStart();

    struct timespec shorttime;
    shorttime.tv_sec = 3;
    shorttime.tv_nsec = 0;

    clock_nanosleep(CLOCK_MONOTONIC, 0, &shorttime, NULL);

    espeak->requestSynth(message, _language);
}

void BillyBass::replay()
{
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
    int j;
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
