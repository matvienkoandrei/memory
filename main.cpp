#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "GameplayManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QCoreApplication::setOrganizationName("Andrew Matvienko"); // this is needed for QSettings
    QCoreApplication::setApplicationName("Memory");            // this is needed for QSettings

    QGuiApplication app(argc, argv);

    GameplayManager gameplayManager;

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("gameplayManager", &gameplayManager);
    engine.rootContext()->setContextProperty("cardsFieldModel", gameplayManager.getCardsFieldModel());

    engine.load(url);

    return app.exec();
}
