#pragma once

#include <QFile>
#include <QMap>
#include <QObject>
#include <QDir>
#include <QQuickItem>
#include <QQmlProperty>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStandardPaths>

#include "../../macros.h"

namespace core::gui::scripts {
  class config: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString group MEMBER _groupName NOTIFY groupChanged CONSTANT)

   public:
    explicit config() {
      QObject::connect(this, &config::groupChanged, this, &config::onGroupChanged);
    }

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

    Q_INVOKABLE
    static
    QString
      getStandardPath(
        _p(code, int)
      );

    void
      setGroup(
        _p(groupName, QString const&)
      );

    static
    core::gui::scripts::config *
      getGroup(
        _p(groupName, QString const&)
      );

   public slots:
    void onGroupChanged(QString const&);

   signals:
    void groupChanged(QString const&);

   private:
    _p(_groupName, QString);
    _p(_group    , core::gui::scripts::config *) = nullptr;

    _p(_file, QFile *) = nullptr;
    _p(_data, QJsonObject);

    inline static _p(_objects, QMap<QString, core::gui::scripts::config *>){};
  };
}

Q_DECLARE_METATYPE(core::gui::scripts::config *)