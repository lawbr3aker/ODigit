#include "./config.hpp"

void
core::gui::scripts::config::load(
        _p(path  , QString const&),
        _p(backup, QString const&)
) {
  _file = new QFile(path);

  if (!_file->exists() and not backup.isEmpty()) {
    QDir().mkpath(QFileInfo(path).absolutePath());

    if (!_file->open(QIODevice::WriteOnly)) {
      QMessageBox::critical(
          nullptr,
          "Config Error",
          QString("Failed to create config file:\n%1\n\nReason:\n%2")
              .arg(path)
              .arg(_file->errorString())
      );
      return;
    }

    // write backup data
    auto temp = new QFile(backup);

    if (!temp->open(QIODevice::ReadOnly)) {
      QMessageBox::critical(
          nullptr,
          "Config Error",
          QString("Failed to open backup file:\n%1\n\nReason:\n%2")
              .arg(backup)
              .arg(temp->errorString())
      );

      _file->close();
      return;
    }

    auto data = temp->readAll();

    if (_file->write(data) != data.size()) {
      QMessageBox::critical(
          nullptr,
          "Config Error",
          QString("Failed to write config file:\n%1\n\nReason:\n%2")
              .arg(path)
              .arg(_file->errorString())
      );

      temp->close();
      _file->close();
      return;
    }

    temp->close();

    _file->flush();
    _file->close();
  }

  // reopen file with readonly
  if (!_file->open(QIODevice::ReadOnly)) {
    QMessageBox::critical(
        nullptr,
        "Config Error",
        QString("Failed to open config file:\n%1\n\nReason:\n%2")
            .arg(path)
            .arg(_file->errorString())
    );
    return;
  }

  auto json = _file->readAll();

  QJsonParseError error{};
  auto document = QJsonDocument::fromJson(json, &error);

  if (error.error != QJsonParseError::NoError) {
    QMessageBox::critical(
        nullptr,
        "Config Error",
        QString("Invalid JSON in config file:\n%1\n\nReason:\n%2")
            .arg(path)
            .arg(error.errorString())
    );

    _file->close();
    return;
  }

  _data = document.object();

  _file->close();
}

void
  core::gui::scripts::config::save(
  ) {
    if (_group)
      return _group->save();

    QJsonDocument document(_data);

    _file->open(QIODevice::WriteOnly);
    _file->write(document.toJson());
    _file->close();
  }

QVariant
  core::gui::scripts::config::value(
    _p(path, QString const&)
  ) const {
    if (_group)
      return _group->value(path);

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
    if (_group)
      return _group->set(path, value);

    QStringList route = path.split("/");

    std::function<QJsonObject(QJsonObject)>
      set_recursive = [&](
        _p(cursor, QJsonObject)
      ) -> QJsonObject {
        QString key = route.front(); route.pop_front();

        cursor[key] = route.length()
          ? set_recursive(cursor.contains(key) ? cursor[key].toObject() : QJsonObject())
          : QJsonValue::fromVariant(value);

        return cursor;
      };

    _data = set_recursive(_data);

    return value;
  }

QString
  core::gui::scripts::config::getStandardPath(
    _p(code, int)
  ) {
    return QString::fromStdString(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first().toStdString());
  }

void
  core::gui::scripts::config::onGroupChanged(
    _p(groupName, QString const&)
  ) {
    if (core::gui::scripts::config::_objects.contains(groupName))
      _group = core::gui::scripts::config::_objects.value(groupName);
    else
      core::gui::scripts::config::_objects.insert(groupName, this);
  }

void
  core::gui::scripts::config::setGroup(
    _p(groupName, QString const&)
  ) {
    _groupName = groupName;
    onGroupChanged(groupName);
  }

core::gui::scripts::config *
  core::gui::scripts::config::getGroup(
    _p(groupName, QString const&)
  ) {
    if (core::gui::scripts::config::_objects.contains(groupName))
      return core::gui::scripts::config::_objects.value(groupName);
  }
