#pragma once

#include <QObject>
#include <QFileDialog>
#include <QStandardPaths>
#include <QTextStream>
#include <QDebug>

#include <random>

#include "../../utils/hash.hpp"

#include "../../macros.h"

namespace core::gui::scripts {
  class registration: public QObject {
    Q_OBJECT

   public:
    Q_INVOKABLE static
    QString
      get_id();

    Q_INVOKABLE static
    void
      write_id();

    Q_INVOKABLE static
    bool
      check_key(
        _p(key , QString const&),
        _p(seed, QString const&)
      );
  };
}