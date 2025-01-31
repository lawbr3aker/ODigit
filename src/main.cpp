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
  qmlRegisterType<core::gui::components::editor>   ("scripts", 1, 0, "Editor");
  qmlRegisterType<core::gui::components::ruler>    ("scripts", 1, 0, "Ruler");
  //
  qmlRegisterType<core::gui::scripts::process>     ("scripts", 1, 0, "Process");
  qmlRegisterType<core::gui::scripts::config>      ("scripts", 1, 0, "Config");
  qmlRegisterType<core::gui::scripts::registration>("scripts", 1, 0, "Registration");

  qRegisterMetaType<core::gui::components::editor_elements::segment *>("core::gui::components::editor_elements::segment *");

  _p(format, QSurfaceFormat);
     format.setSwapInterval(0);
  //
  QSurfaceFormat::setDefaultFormat(format);

  auto config = new core::gui::scripts::config();
  config->load(core::gui::scripts::config::getStandardPath(17) + "/../Optitex Pattern Design/ODigit/config.json", ":/Assets/Statics/config");
  config->setGroup("config-main");

  const auto language = config->value("interface/language").toString();
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
    [url, &app](QObject *obj, const QUrl &objUrl) {
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
