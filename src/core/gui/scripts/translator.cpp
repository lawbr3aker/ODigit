#include "./translator.hpp"

void
  core::gui::scripts::translator::load(
    _p(path, QString const&)
  ) {
    _file = new QFile(path);
    if (!_file->exists()) {
      return;
    }
    //
    _file->open(QIODevice::ReadOnly);
    _data = QJsonDocument::fromJson(_file->readAll()).object();
    _file->close();
  }

QVariant
  core::gui::scripts::translator::tr(
    _p(path, QString const&)
  ) const {
    QStringList route = path.split("/");

    QJsonObject cursor = _data;
    for (const QString &key: route) {
      if (cursor.contains(key)) {
        QJsonValue value = cursor[key];
        if (value.isObject())
          cursor = value.toObject();
        else
          return value.toString();
      } else
        break;
    }

    return {};
  }
