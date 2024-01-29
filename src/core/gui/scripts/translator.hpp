#pragma once

#include <QFile>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>

#include "../../macros.h"

namespace core::gui::scripts {
  class translator: public QObject {
    Q_OBJECT
    Q_PROPERTY(core::gui::scripts::translator * global MEMBER global NOTIFY globalChanged)

   public:
    inline static
    core::gui::scripts::translator * global;

   public:
    Q_INVOKABLE
    void
      load(
        _p(path, QString const&)
      );

    [[nodiscard]]
    Q_INVOKABLE
    QVariant
      tr(
        _p(path, QString const&)
      ) const;

  signals:
    void globalChanged();

   private:
    _p(_file, QFile *) = nullptr;
    _p(_data, QJsonObject);
  };
}

Q_DECLARE_METATYPE(core::gui::scripts::translator *)
