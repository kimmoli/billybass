/*
    billybass, Billy Bass
*/


#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include <QtQml>
#include <QScopedPointer>
#include <QQuickView>
#include <QQmlEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <QCoreApplication>
#include "espeak.h"


int main(int argc, char *argv[])
{
    qmlRegisterType<Espeak>("billybass.Espeak", 1, 0, "Espeak");

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());
    view->setSource(SailfishApp::pathTo("qml/billybass.qml"));
    view->show();

    return app->exec();
}

