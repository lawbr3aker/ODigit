#pragma once

#include <QObject>
#include <QVariant>
#include <QDebug>

#include <fstream>
#include <nlohmann/json.hpp>

using namespace nlohmann;

class Config : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Config * global MEMBER global)
public:
    static inline Config * global;

    json data;

    Q_INVOKABLE QVariant value(QString const& path, QString const& type) {
        auto const& pointer = json::json_pointer(path.toStdString());

        if (data.contains(pointer)) {
            auto const &ref = data[pointer];

            if (type == "int")
                return (int) ref;
            if (type == "double")
                return (double) ref;
            if (type == "string")
                return (QString) ((json::string_t) ref).c_str();
        }

        return "";
    }
    
    Q_INVOKABLE void set(QString const& path, QVariant const& value, QString const& type) {
        auto & ref = data[json::json_pointer(path.toStdString())];

        if (type == "int")
            ref = value.value<int>();
        else
        if (type == "double")
            ref = value.value<double>();
        else
        if (type == "string")
            ref = value.value<QString>().toStdString();
    }

    Q_INVOKABLE Config & load(QString const& path) {
        _path = path.toStdString();
        _file.open(_path, std::ios::in);
        if (!_file) {
            _file.close();
            _file.open(_path, std::ios::out);
            _file << "{}";
            _file.close();
            _file.open(_path, std::ios::in);
        }

        data = json::parse(_file);

        return *this;
    }

    Q_INVOKABLE void reload() {
        _file.close();
        _file.open(_path, std::ios::in);
        data = json::parse(_file);
    }

    Q_INVOKABLE void save() {
        _file.close();
        _file.open(_path, std::ios::out);
        _file << data.dump(2).c_str() << "\n";

        reload();
    }

private:
    std::string _path;
    std::fstream _file;
};

Q_DECLARE_METATYPE(Config *)
