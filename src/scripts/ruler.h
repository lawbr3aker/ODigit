#pragma once

#include <QImage>
#include <QPixmap>
#include <QList>
#include <QDebug>
#include <QPainter>
#include <QQuickPaintedItem>
#include <QQmlEngine>

#include <opencv2/opencv.hpp>
#include <cmath>

class Ruler : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int orientation MEMBER orientation)
    Q_PROPERTY(float unitPixels MEMBER unitPixels)
    Q_PROPERTY(int offset MEMBER offset)
public:
    int orientation = Qt::Horizontal;
    float unitPixels = 56;
    int offset = 0;

    void paint(QPainter *painter) override {
        auto h = (int) height();
        auto w = (int) width();

        auto b = 1;
        auto u = unitPixels;
        while (u < 40) {
            u *= 5;
            b *= 5;
        }

        auto step = u / 10.;

        auto o = offset % (int) u - u;
        bool c = true;
        auto s = (int) std::trunc(-offset / u) - 1;

        QFont font;
        font.setWeight(QFont::Bold);
        font.setFamily("sans-serif");
        if (orientation == Qt::Vertical)
            font.setPixelSize((int) (w / 2.3));
        else
            font.setPixelSize((int) (h / 2.3));
        painter->setFont(font);

        do {
            for (int i = 0; i < 10; ++i) {
                int x, y;
                if (orientation == Qt::Horizontal) {
                    x = o;
                    if (o >= w) {
                        c = false;
                        break;
                    }

                    if (i == 0) {
                        painter->drawLine(x, h, x, 0);
                        painter->drawText(x + 3, (int) (h / 2.5), std::to_string(s++ * b).c_str());
                    }
                    else if (i == 3 || i == 7)
                        painter->drawLine(x, h, x, (int) (h * .75));
                    else if (i == 5)
                        painter->drawLine(x, h, x, (int) (h * .5));
                    else
                        painter->drawLine(x, h, x, (int) (h * .85));
                } else if (orientation == Qt::Vertical) {
                    y = (int) o;
                    if (o >= h) {
                        c = false;
                        break;
                    }

                    if (i == 0) {
                        painter->drawLine(w, y, 0, y);
                        painter->rotate(-90);
                        painter->drawText(-y + 3, (int) (w / 2.5), std::to_string(s++ * b).c_str());
                        painter->rotate(90);
                    }
                    else if (i == 3 || i == 7)
                        painter->drawLine(w, y, (int) (w * .75), y);
                    else if (i == 5)
                        painter->drawLine(w, y, (int) (w * .5), y);
                    else
                        painter->drawLine(w, y, (int) (w * .85), y);
                }
                o += step;
            }
        } while (c);
    }
};
