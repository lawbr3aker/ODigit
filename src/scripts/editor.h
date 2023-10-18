#pragma once

#include <QImage>
#include <QPixmap>
#include <QList>
#include <QDebug>
#include <QPainter>
#include <QQuickPaintedItem>

#include <opencv2/opencv.hpp>

struct Line {
    QPoint &s;
    QPoint &e;
};

class Editor : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QList<QPoint> points READ points WRITE setPoints)
public:
    Editor(Editor const& other, QQuickItem *parent=nullptr)
    : QQuickPaintedItem(parent) {
        Q_UNUSED(other)
    }
    explicit Editor(QQuickItem *parent=nullptr)
    : QQuickPaintedItem(parent) {
        _points << QPoint(0, 0) << QPoint(100, 100);
        _lines.push_back(new Line{ _points[0], _points[1]});
    }

    void setImage(cv::Mat const& buffer) {
        qDebug() << "Set\n";
        auto image = QImage((uchar*) buffer.data, buffer.cols, buffer.rows, QImage::Format_RGB888);
        _image = QPixmap::fromImage(image);
        qDebug() << _image.width();
    }

    void setPoints(QList<QPoint> const& points) {
        _points = points;
    }

    [[nodiscard]] QList<QPoint> const& points() const {
        return _points;
    }

    void paint(QPainter *painter) override {
        painter->drawPixmap(QPoint(0, 0), _image);

        for (Line const* line : _lines) {
            painter->drawLine(line->s, line->e);
        }
    }
signals:
private:
    QPixmap _image;
    QList<QPoint> _points;
    QList<Line *> _lines;
};
