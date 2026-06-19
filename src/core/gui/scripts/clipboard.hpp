#pragma once

#include <QFile>
#include <QMap>
#include <QObject>
#include <QQuickItem>
#include <QQmlProperty>
#include <QJsonObject>
#include <QClipboard>
#include <QJsonDocument>
#include <QStandardPaths>

#include "../../macros.h"

namespace core::gui::scripts {
  class clipboard: public QObject {
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT

   public:
    static
    void
      setClipboard(
        _p(clipboard, QClipboard *)
      );

    Q_INVOKABLE
    static
    QString
      getText(
      );

   private:
    inline static QClipboard * _clipboard = nullptr;
  };
}

Q_DECLARE_METATYPE(core::gui::scripts::clipboard *)
