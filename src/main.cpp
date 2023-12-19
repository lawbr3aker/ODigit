#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QSplashScreen>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QStyleFactory>
#include <QDebug>
#include <QStandardPaths>
#include <QTextCodec>
#include <QLabel>

#include "scripts/process.h"
#include "scripts/editor.h"
#include "scripts/config.h"
#include "scripts/translator.h"
#include "scripts/ruler.h"

extern int main(int argc, char *argv[]) {
    // system("chcp 65001");
    QApplication app(argc, argv);

    QPixmap image;
    image.load(":/assets/images/banner");
    image = image.scaledToWidth(500);
    QSplashScreen splash(image);
    splash.show();
    app.processEvents();
    app.setWindowIcon(QIcon(":/icon.ico"));
    QApplication::addLibraryPath(".");
    // QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    qmlRegisterType<Process>("scripts", 1, 0, "Process");
    qmlRegisterType<Editor>("scripts", 1, 0, "Editor");
    qmlRegisterType<Config>("scripts", 1, 0, "Config");
    qmlRegisterType<Translator>("scripts", 1, 0, "Translator");
    qmlRegisterType<Ruler>("scripts", 1, 0, "Ruler");

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    //
    QFile default_config(":/config");
    QFileInfo path(QFileInfo(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first()).path() + "/Optitex Pattern Design/ODigit/config.json");
    default_config.open(QIODevice::ReadOnly);
    auto dir = QDir(QFileInfo(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first()).path());
    if (!path.exists()) {
        dir.mkpath("Optitex Pattern Design/ODigit");
    }
    Config::global = &(new Config())->load(path.filePath(), default_config);

    auto const &language = Config::global->value("interface/language").value<QString>();
    Translator::global = &(new Translator())->load((":/translations/" + language).toStdString().c_str());

    if (language == "fa") {
        QString family = QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/assets/fonts/Peyda-Bold")).at(0);
        QFont font(family, 10, QFont::Bold);
        QApplication::setFont(font);
    } else if (language == "en") {
        QApplication::setFont({"Consolas", 10, QFont::Bold});
    }

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
              if (!obj && url == objUrl)
                  QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
    engine.load(url);
    splash.hide();
    return QApplication::exec();
}
