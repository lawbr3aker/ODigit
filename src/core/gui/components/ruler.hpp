#pragma once

#include <QPainter>
#include <QQuickPaintedItem>

#include <cmath>

#include "../../macros.h"

namespace core::gui::components {
  class ruler: public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(int orientation MEMBER orientation)
    Q_PROPERTY(int unit_pixels MEMBER unit_pixels)
    Q_PROPERTY(int offset      MEMBER offset)

   public:
    void
      paint(
        _p(painter, QPainter *)
      ) override;

   public:
    _p(orientation, int);
    _p(unit_pixels, float);
    _p(offset     , int) = 0;
  };
}

Q_DECLARE_METATYPE(core::gui::components::ruler *)