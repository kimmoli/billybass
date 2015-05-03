/*
    billybass, Billy Bass
*/

#include "espeak.h"
#include <QDebug>
#include <QThread>

Espeak::Espeak(QObject *parent) :
    QObject(parent)
{
    emit versionChanged();
    _libespeakVersion = QString();
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

}

Espeak::~Espeak()
{
    if(iphbdHandler)
        (void)iphb_close(iphbdHandler);

    if(iphbNotifier)
        delete iphbNotifier;

    terminate();
}

QString Espeak::readVersion()
{
    return APPVERSION;
}

/* IPHB stuff */

void Espeak::heartbeatReceived(int sock)
{
    Q_UNUSED(sock);

    qDebug() << "iphb heartbeat";

    iphbStop();

    if (_espeakInitialized)
        iphbStart();
}

void Espeak::iphbStart()
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

void Espeak::iphbStop()
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

void Espeak::synth(QString text)
{
    if (!_espeakInitialized)
        init();

    if (text.isEmpty())
    {
        qDebug() << "not synthesizing empty string";
        return;
    }

    qDebug() << "synth:" << text;

    espeak_ERROR ret = espeak_Synth(text.toLatin1().data(), text.length()+1, 0, POS_CHARACTER, 0, _synthFlags, NULL, NULL);
    if (ret != EE_OK)
    {
        qDebug() << "synth failed" << ret;
    }

    _lastStringSynth = text;

    terminate();
}

void Espeak::init()
{
    const char *version;
    const char *path_data;
    int sampleRate;

    while (_terminating);

    if (_espeakInitialized)
        terminate();

    qDebug() << "initialising...";

    sampleRate = espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0);

    if (_libespeakVersion.isEmpty())
    {
        version = espeak_Info(&path_data);
        qDebug() << "samplerate:" << sampleRate;
        qDebug() << "version:" << QString(version);
        qDebug() << "data path:" << QString(path_data);

        _libespeakVersion = QString(version);
        emit libespeakVersionChanged();
    }

    _espeakInitialized = true;

    setLanguage(_language);
}

void Espeak::setLanguage(QString language)
{
    QString lang = language;

    if (lang.isEmpty())
        lang = QLocale::system().name().split('_').at(0);

    qDebug() << "setting language to" << lang;

    if(_espeakInitialized)
    {
        if (espeak_SetVoiceByName(lang.toLocal8Bit().data()) != EE_OK)
        {
            qDebug() << "language set failed, fallback to english";
            lang= "english";
            if (espeak_SetVoiceByName(lang.toLocal8Bit().data()) != EE_OK)
            {
                qDebug() << "language fallback failed.";
                return;
            }
        }
    }

    _language = lang;
    emit languageChanged();
}

void Espeak::speakNotification(QString message)
{
    if (!_espeakInitialized)
        init();

    /* Allow system notification sound to be played */
    iphbStart();

    struct timespec shorttime;
    shorttime.tv_sec = 3;
    shorttime.tv_nsec = 0;

    clock_nanosleep(CLOCK_MONOTONIC, 0, &shorttime, NULL);

    synth(message);
}

void Espeak::replay()
{
    synth(_lastStringSynth);
}

void Espeak::terminate()
{
    _terminating = true;
    if (_espeakInitialized)
    {
        do
        {
            QThread::msleep(100);
        }
        while (espeak_IsPlaying());

        espeak_Terminate();

        QThread::msleep(100);

        _espeakInitialized = false;

        qDebug() << "terminated";
    }
    _terminating = false;
}

QVariantList Espeak::getVoices()
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
