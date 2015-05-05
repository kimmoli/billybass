#include "notificationmanager.h"

#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QSettings>
#include <QDBusInterface>
#include <QDBusPendingReply>

class NotificationManagerPrivate
{
    Q_DECLARE_PUBLIC(NotificationManager)

public:
    NotificationManagerPrivate(NotificationManager *q)
        : q_ptr(q),
          interface(NULL),
          connected(false)
    { /*...*/ }

    NotificationManager *q_ptr;

    QDBusInterface *interface;

    bool connected;
};

NotificationManager::NotificationManager(QObject *parent)
    : QObject(parent), d_ptr(new NotificationManagerPrivate(this))
{
    Q_D(NotificationManager);
    QDBusConnection::sessionBus().registerObject("/org/freedesktop/Notifications", this, QDBusConnection::ExportAllSlots);

    d->interface = new QDBusInterface("org.freedesktop.DBus",
                                      "/org/freedesktop/DBus",
                                      "org.freedesktop.DBus");

    d->interface->call("AddMatch", "interface='org.freedesktop.Notifications',member='Notify',type='method_call',eavesdrop='true'");

    this->initialize();
}

NotificationManager::~NotificationManager()
{
    Q_D(NotificationManager);
    delete d;
}


void NotificationManager::initialize()
{
    Q_D(NotificationManager);
    bool success = false;

    if(d->interface->isValid())
    {
        success = true;
        printf("Connected to Notifications D-Bus service.\n");
    }

    if(!(d->connected = success))
    {
        QTimer::singleShot(2000, this, SLOT(initialize()));
        printf("Failed to connect to Notifications D-Bus service.\n");
    }
}

QDBusInterface* NotificationManager::interface() const
{
    Q_D(const NotificationManager);
    return d->interface;
}

uint NotificationManager::Notify(const QString &app_name, uint replaces_id, const QString &app_icon,
                                 const QString &summary, const QString &body, const QStringList &actions,
                                 const QVariantHash &hints, int expire_timeout)
{
    Q_UNUSED(replaces_id);
    Q_UNUSED(app_icon);
    Q_UNUSED(actions);
    Q_UNUSED(expire_timeout);
    Q_UNUSED(app_name);

    QString category = hints.value("category", "").toString();
    QString subject = hints.value("x-nemo-preview-summary", "").toString();
    QString data = hints.value("x-nemo-preview-body", "").toString();

    // Avoid sending a reply for this method call, since we've received it because we're eavesdropping.
    // The actual target of the method call will send the proper reply.

    Q_ASSERT(calledFromDBus());
    setDelayedReply(true);

    qDebug() << "category:" << category << "subject:" << subject;
    qDebug() << "summary:" << summary << "data:" << data;
    qDebug() << "body:" << body;

    if (!body.isEmpty())
        emit this->gotNotification(body);
    else if (!summary.isEmpty())
        emit this->gotNotification(summary);
    else
        qDebug() << "ignoring this notification";

//    else if (!data.isEmpty())
//        emit this->gotNotification(data);
//    else if (!subject.isEmpty())
//        emit this->gotNotification(subject);

    return 0;
}
