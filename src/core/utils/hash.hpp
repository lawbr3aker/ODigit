#pragma once

#include <random>

#include <QString>
#include <QCryptographicHash>

#include "../macros.h"

namespace core::utils::hash {
  QString
    random(
      _p(input , QString const&),
      _p(seed  , int),
      _p(method, QCryptographicHash::Algorithm)
    );

  QString
    hash(
      _p(input , QString const&),
      _p(method, QCryptographicHash::Algorithm)
    );
}