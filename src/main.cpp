#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStyleFactory>
#include "version.h"

#include "scripts/process.h"
#include "scripts/editor.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif


    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));
    app.setApplicationName(QString::fromLocal8Bit(APPLICATION_NAME));
    app.setApplicationVersion(QString::fromLocal8Bit(APPLICATION_VERSION_STRING_FULL));
    app.setOrganizationName(QString::fromLocal8Bit(APPLICATION_COMPANY_NAME));
    app.setOrganizationDomain(QString::fromLocal8Bit(APPLICATION_COMPANY_DOMAIN));

    qmlRegisterType<Process>("scripts", 1, 0, "Process");
    qmlRegisterType<Editor>("scripts", 1, 0, "Editor");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return qApp->exec();
}
