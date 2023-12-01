#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QQmlApplicationEngine>
#include <QStyleFactory>
#include <QDebug>
#include <QTextCodec>
#include <QLabel>
#include <QTranslator>

#include "scripts/process.h"
#include "scripts/editor.h"
#include "scripts/config.h"
#include "scripts/translator.h"
#include "scripts/ruler.h"

extern int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::addLibraryPath(".");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    qmlRegisterType<Process>("scripts", 1, 0, "Process");
    qmlRegisterType<Editor>("scripts", 1, 0, "Editor");
    qmlRegisterType<Config>("scripts", 1, 0, "Config");
    qmlRegisterType<Translator>("scripts", 1, 0, "Translator");
    qmlRegisterType<Ruler>("scripts", 1, 0, "Ruler");
    qDebug() << "begin";
    QImage image("C:\\Users\\lawbr3aker\\Desktop\\IMG_1689.png");
    QLabel myLabel;
    myLabel.setBaseSize({200, 200});
    myLabel.setPixmap(QPixmap::fromImage(image));

    myLabel.show();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    Config::global = &(new Config())->load("config.json");

    auto const &language = (std::string) Config::global->data["/interface/language"_json_pointer];

    Translator::global = &(new Translator())->load((std::string(":/translations/") + language).c_str());

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
    qDebug() << "load";
    engine.load(url);
    qDebug() << "loaded";

    return QApplication::exec();
}
