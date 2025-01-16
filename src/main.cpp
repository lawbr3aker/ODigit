#include <windows.h>

#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QSplashScreen>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QStyleFactory>
#include <QDebug>
#include <QStandardPaths>
#include <QSurfaceFormat>
#include <QQuickView>
#include <QTextCodec>

#include "core/gui/components/ruler.hpp"
#include "core/gui/components/editor.hpp"

#include "core/gui/scripts/process.hpp"
#include "core/gui/scripts/config.hpp"
#include "core/gui/scripts/translator.hpp"
#include "core/gui/scripts/registration.hpp"

#include "core/utils/hash.hpp"


#ifdef NDEBUG
#define SPLASH
#endif

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

#ifdef SPLASH
  _p(image, QPixmap)(":/Assets/Images/banner");
     image = image.scaledToWidth(850);
  //
  _p(splash, QSplashScreen)(image);
     splash.show();
  //
  QApplication::processEvents();
#endif

  QApplication::setWindowIcon(QIcon(":/Assets/Images/icon"));
  QApplication::addLibraryPath(".");
  QApplication::setStyle(QStyleFactory::create("Fusion"));
  //
  qmlRegisterType<core::gui::components::editor> ("scripts", 1, 0, "Editor");
  qmlRegisterType<core::gui::components::ruler>  ("scripts", 1, 0, "Ruler");
  //
  qmlRegisterType<core::gui::scripts::process>     ("scripts", 1, 0, "Process");
  qmlRegisterType<core::gui::scripts::config>      ("scripts", 1, 0, "Config");
  qmlRegisterType<core::gui::scripts::translator>  ("scripts", 1, 0, "Translator");
  qmlRegisterType<core::gui::scripts::registration>("scripts", 1, 0, "Registration");

  _p(format, QSurfaceFormat);
     format.setSwapInterval(0);
  //
  QSurfaceFormat::setDefaultFormat(format);

  // read default config file content
  _p(config_default, QFile)(":/Assets/Statics/config");
     config_default.open(QFile::ReadOnly);
  // get config file path
  _p(app_data_path, QDir)(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first());
     app_data_path.cdUp();

  QFileInfo config_path(app_data_path.path() + "/Optitex Pattern Design/ODigit/config.json");

  if (not config_path.exists())
    app_data_path.mkpath("Optitex Pattern Design/ODigit");
  // create global config
  _p(config_global, core::gui::scripts::config);
     config_global.load(config_path.filePath(), config_default.readAll());
  //
  core::gui::scripts::config::global = &config_global;

  auto language = config_global.value("interface/language").value<QString>();

  _p(translator_global, core::gui::scripts::translator);
     translator_global.load(":/Translations/" + language);
  //
  core::gui::scripts::translator::global = &translator_global;

  if (language == "fa") {
    QApplication::setFont({
      QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/Assets/Fonts/Default-FA")
      ).at(0),
      9,
      QFont::Bold
    });
  } else if (language == "en") {
    QApplication::setFont({
      QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont(":/Assets/Fonts/Default-EN")
      ).at(0),
      9,
      QFont::Bold
    });
  }

  _p(engine, QQmlApplicationEngine);
  _p(url, QUrl)("qrc:/main.qml");

  QObject::connect(
    &engine, &QQmlApplicationEngine::objectCreated, &app,
    [url](QObject *obj, const QUrl &objUrl) {
      if (!obj && url == objUrl)
        QCoreApplication::exit(-1);
      },
    Qt::QueuedConnection
  );

#ifdef SPLASH
  Sleep(1000);
  engine.load(url);
  splash.hide();
#else
  engine.load(url);
#endif

  return QApplication::exec();
}
