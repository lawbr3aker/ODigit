#pragma once

#include <QImage>
#include <QPixmap>
#include <QList>
#include <QDebug>
#include <QPainter>
#include <QQuickPaintedItem>
#include <QQmlEngine>

#include <opencv2/opencv.hpp>
#include <utility>

class Elem: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString status WRITE setStatus READ getStatus)

public:
    enum Status {
        Hover, Active, Default
    };
public:
    Status status = Default;

    QString setStatus(QString const& s = "") {
        if (s == "hover")
            status = Hover;
        else if (s == "active")
            status = Active;
        else
            status = Default;

        return getStatus();
    }

    [[nodiscard]] QString getStatus() const {
        switch (status) {
            case Hover  : return "hover";
            case Active : return "active";
            default: return "default";
        }
    }
};


class Point: public Elem {
    Q_OBJECT
    Q_PROPERTY(double x MEMBER x)
    Q_PROPERTY(double y MEMBER y)
public:
    double x;
    double y;

    Point(double x, double y)
    : x(x), y(y) {
    }

    Point(Point const& other)
    : x(other.x), y(other.y) {
    }
};

class Text: public QObject {
    Q_OBJECT
    Q_PROPERTY(int     x      MEMBER x)
    Q_PROPERTY(int     y      MEMBER y)
    Q_PROPERTY(int     w      MEMBER w)
    Q_PROPERTY(int     h      MEMBER h)
    Q_PROPERTY(float   scale  MEMBER scale)
    Q_PROPERTY(QString text   MEMBER text)
    Q_PROPERTY(int     size   MEMBER size)
    Q_PROPERTY(bool    bold   MEMBER bold)
    Q_PROPERTY(bool    italic MEMBER italic)
public:
    int x;
    int y;
    int w;
    int h;
    float scale;
    QString text;
    QString family;
    int     size;
    bool    bold;
    bool    italic;

    Text(int x, int y, int w, int h, float scale, QString text, QString family, int size, bool bold, bool italic)
    : x(x), y(y), w(w), h(h), scale(scale),
      text(std::move(text)), family(std::move(family)), size(size), bold(bold), italic(italic) {
    }

    Text(Text const& other)
    : x(other.x), y(other.y), w(other.w), h(other.h), scale(other.scale),
      text(other.text), family(other.family), size(other.size), bold(other.bold), italic(other.italic) {
    }
};

Q_DECLARE_METATYPE(Text *)

class Line: public Elem {
    Q_OBJECT
    Q_PROPERTY(Point * s MEMBER s)
    Q_PROPERTY(Point * e MEMBER e)
public:
    Point * s = nullptr;
    Point * e = nullptr;

    Line(Point *& s, Point *& e)
    : s(s), e(e) {
    }

    Line(Line const& other)
    : s(other.s), e(other.e) {
    }
};

Q_DECLARE_METATYPE(Line *)

class Editor : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QList<Point *> points MEMBER points NOTIFY pointsChanged)
    Q_PROPERTY(QList<Line *>  lines  MEMBER lines NOTIFY linesChanged)
    Q_PROPERTY(QList<Text *>  texts  MEMBER texts NOTIFY textsChanged)
    Q_PROPERTY(float        zoom   MEMBER zoom NOTIFY zoomChanged)
    Q_PROPERTY(QPoint       pan    MEMBER pan NOTIFY panChanged)
public:
    QList<Point *> points;
    QList<Line *>  lines;
    QList<Text *>  texts;
    float        zoom = 1;
    QPoint       pan = QPoint(0, 0);

    Editor(Editor const& other, QQuickItem *parent=nullptr)
    : QQuickPaintedItem(parent) {
        Q_UNUSED(other)
    }
    explicit Editor(QQuickItem *parent=nullptr)
    : QQuickPaintedItem(parent) {
    }

    void setImage(cv::Mat const& buffer) {
        cv::Mat temp; // make the same cv::Mat
        cvtColor(buffer, temp,cv::COLOR_BGR2RGB);
        QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
        dest.bits();
        _image = QPixmap::fromImage(dest);
        this->update();
    }

    void setZoom(float zoom) {
        this->zoom = zoom;
        emit zoomChanged();
    }

    Q_INVOKABLE Text * addText(int x, int y, int w, int h, float scale, QString const& text, QString const& family, int size, bool bold, bool italic) {
        texts.push_back(new Text(x, y, w, h, scale, text, family, size, bold, italic));

        QQmlEngine::setObjectOwnership(texts.back(), QQmlEngine::CppOwnership);
        return texts.back();
    }

    Q_INVOKABLE Point * addPoint(double x, double y) {
        points.push_back(new Point(x, y));

        QQmlEngine::setObjectOwnership(points.back(), QQmlEngine::CppOwnership);
        return points.back();
    }

    Q_INVOKABLE Line * addLine(Point * s, Point * e) {
        lines.push_back(new Line(s, e));

        QQmlEngine::setObjectOwnership(lines.back(), QQmlEngine::CppOwnership);
        return lines.back();
    }

    Q_INVOKABLE void removeLine(Line * line) {
        lines.erase(std::find(lines.begin(), lines.end(), line));
    }

    Q_INVOKABLE void removePoint(Point * point) {
        points.erase(std::find(points.begin(), points.end(), point));
    }

    void paint(QPainter *painter) override {
        painter->drawPixmap(pan,_image.scaled(_image.width() * zoom, _image.height() * zoom, Qt::KeepAspectRatio));

        painter->setPen(QPen(QColor(55, 173, 118), 4));
        for (auto line : lines) {
            painter->drawLine(QPoint{int(line->s->x * zoom + pan.x()), int(line->s->y * zoom + pan.y())},
                              QPoint{int(line->e->x * zoom + pan.x()), int(line->e->y * zoom + pan.y())});
        }

        painter->setPen(QPen(QColor(16, 138, 227), 1));
        painter->setBrush(QColor(16, 138, 227));

        const int r = 12;
        for (auto state : {
                Elem::Status::Default,
                Elem::Status::Active,
                Elem::Status::Hover
        }) {
            switch (state) {
                case Elem::Default: painter->setBrush(QColor(16, 138, 227)); break;
                case Elem::Active : painter->setBrush(QColor(73, 33, 35)); break;
                case Elem::Hover  : painter->setBrush(QColor(7, 56, 92)); break;
                default:;
            }
            for (auto const& point : points) {
                if (point->status == state) {
                    const double x = point->x * zoom + pan.x();
                    const double y = point->y * zoom + pan.y();

                    painter->drawEllipse(int(x - r/2.), int(y - r/2.), r, r);
                }
            }
        }

        for (auto const& text: texts) {
            QFont font;
            font.setFamily(text->family);
            double s = zoom / text->scale;
            font.setPixelSize(s * text->size);
            font.setBold(text->bold);
            font.setItalic(text->italic);
            painter->setFont(font);
            painter->drawText((int) (s * text->x + pan.x()), (int) (s * text->y + pan.y()), (int) (s * text->w), (int) (s * text->h), 0, text->text);
        }
    }
signals:
    void linesChanged();
    void pointsChanged();
    void textsChanged();
    void zoomChanged();
    void panChanged();
private:
    QPixmap _image;
};
