#include "./config.hpp"

void
  core::gui::scripts::config::load(
    _p(path  , QString const&),
    _p(backup, QString const&)
  ) {
    _file = new QFile(path);
    if (!_file->exists()) {
      _file->open(QIODevice::WriteOnly);
      // write backup data
      _file->write(backup.toStdString().c_str());
      _file->close();
    }
    // reopen file with readonly
    _file->open(QIODevice::ReadOnly);
    _data = QJsonDocument::fromJson(_file->readAll()).object();
    _file->close();
  }

void
  core::gui::scripts::config::save(
  ) {
    QJsonDocument document(_data);

    _file->open(QIODevice::WriteOnly);
    _file->write(document.toJson());
    _file->close();
  }

QVariant
  core::gui::scripts::config::value(
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
          return value.toVariant();
      } else
        break;
    }

    return {};
  }

QVariant
  core::gui::scripts::config::set(
    _p(path , QString const&),
    _p(value, QVariant const&)
  ) {
    QStringList route = path.split("/");

    std::function<QJsonObject(QJsonObject)>
      set_recursive = [&](
        _p(cursor, QJsonObject)
      ) -> QJsonObject {
        QString const& key = route.front(); route.pop_front();

        cursor[key] = route.length()
          ? set_recursive(cursor[key].toObject())
          : QJsonValue::fromVariant(value);

        return cursor;
      };

    _data = set_recursive(_data);

    return value;
  }