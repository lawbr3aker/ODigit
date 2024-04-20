#include "hash.hpp"

#include <QDebug>

QString
  core::utils::hash::random(
    _p(input , QString const&),
    _p(seed  , int),
    _p(method, QCryptographicHash::Algorithm)
  ) {
    QByteArray randomized;
    std::srand(seed);

    for (auto const& chr : input) {
      randomized.push_back((char) (float(std::rand()) / RAND_MAX * chr.toLatin1()));
    }
  }

QString
  core::utils::hash::hash(
    _p(input , QString const&),
    _p(method, QCryptographicHash::Algorithm)
  ) {
    return QCryptographicHash::hash(input.toUtf8(), method).toHex();
  }