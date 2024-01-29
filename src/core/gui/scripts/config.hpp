#pragma once

#include <QFile>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>

#include "../../macros.h"

namespace core::gui::scripts {
  class config: public QObject {
    Q_OBJECT
    Q_PROPERTY(core::gui::scripts::config * global MEMBER global NOTIFY globalChanged)

   public:
    inline static
    core::gui::scripts::config * global;

   public:
    Q_INVOKABLE
    void
      load(
        _p(path  , QString const&),
        _p(backup, QString const&) = ""
      );

    Q_INVOKABLE
    void
      save();

    [[nodiscard]]
    Q_INVOKABLE
    QVariant
      value(
        _p(path, QString const&)
      ) const;

    Q_INVOKABLE
    QVariant
      set(
        _p(path , QString const&),
        _p(value, QVariant const&)
      );

   signals:
    void globalChanged();

   private:
    _p(_file, QFile *) = nullptr;
    _p(_data, QJsonObject);
  };
}

Q_DECLARE_METATYPE(core::gui::scripts::config *)