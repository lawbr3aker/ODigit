#pragma once

#include <QObject>
#include <QVariant>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTranslator>
#include <QJsonDocument>
#include <QJsonObject>

#include <fstream>

class Config : public QObject {
  Q_OBJECT
    Q_PROPERTY(Config *global MEMBER global)
  public:
    static inline Config *global;

    QJsonObject data;

    Q_INVOKABLE QVariant value(QString const &path, QString const& type="") const {
        auto const& keys = path.split("/");

        QJsonObject ref = data;
        for (int i = 0; i < keys.length() - 1; ++i) {
            ref = ref[keys[i]].toObject();
        }

        if (type != "") {
            if (type == "int")
                return ref[keys.last()].toVariant().value<int>();
            if (type == "double")
                return ref[keys.last()].toVariant().value<double>();
            if (type == "string")
                return ref[keys.last()].toVariant().value<QString>();
        }

        return ref[keys.last()].toVariant();
    }

    QJsonValue setRecursive(QJsonObject ref, QStringList & keys, QJsonValue const& value) {
        QString key = keys.front(); keys.pop_front();

        if (keys.length()) {
            ref[key] = setRecursive(ref[key].toObject(), keys, value);
        } else {
            ref[key] = value;
        }
        return ref;
    }

    Q_INVOKABLE void set(QString const &path, QVariant const &value, QString const &type) {
        auto keys = path.split("/");

        QJsonValue *json_value = nullptr;

        if (type == "int")
            json_value = new QJsonValue(value.value<int>());
        else if (type == "double")
            json_value = new QJsonValue(value.value<double>());
        else if (type == "string")
            json_value = new QJsonValue(value.value<QString>());

        if (json_value) {
            data = setRecursive(data, keys, *json_value).toObject();
        }
    }

    Q_INVOKABLE Config &load(QString const &path, QFile &default_value) {
        _file = new QFile(path);
        if (!_file->exists()) {
            _file->open(QIODevice::WriteOnly);
            default_value.open(QIODevice::ReadOnly);
            _file->write(default_value.readAll());
            _file->close();
        }
        _file->open(QIODevice::ReadOnly);

        data = QJsonDocument::fromJson(_file->readAll()).object();

        return *this;
    }

    Q_INVOKABLE void save() {
        QJsonDocument document(data);

        _file->close();
        _file->open(QIODevice::WriteOnly);
        _file->write(document.toJson());

        _file->close();
    }

  private:
    QFile *_file;
};

Q_DECLARE_METATYPE(Config *)
