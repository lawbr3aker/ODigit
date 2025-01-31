#include "./config.hpp"

void
  core::gui::scripts::config::load(
    _p(path  , QString const&),
    _p(backup, QString const&)
  ) {
    _file = new QFile(path);
    if (!_file->exists() and not backup.isEmpty()) {
      _file->open(QIODevice::WriteOnly);
      // write backup data
      auto temp = new QFile(backup);
      temp->open(QIODevice::ReadOnly);
      _file->write(temp->readAll().toStdString().c_str());
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
