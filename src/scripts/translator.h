#pragma once

#include <QObject>
#include <QVariant>
#include <QDebug>
#include <QTranslator>
#include <QJsonDocument>
#include <QJsonObject>

#include <fstream>

class Translator : public QObject {
  Q_OBJECT
    Q_PROPERTY(Translator *global MEMBER global NOTIFY globalChanged)

  public:
    static inline Translator *global;

    QJsonDocument data;

    Q_INVOKABLE Translator &load(QString const &path) {
        QFile file(path);
        file.open(QIODevice::ReadOnly);

        data = QJsonDocument::fromJson(file.readAll());

        file.close();

        return *this;
    }

    Q_INVOKABLE [[nodiscard]] QVariant tr(QString const &key) const {
        return data[key].toString();
    }

  signals:

    void globalChanged();
};

Q_DECLARE_METATYPE(Translator *)
