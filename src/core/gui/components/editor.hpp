#pragma once

#include <QObject>
#include <QPixmap>
#include <QQuickPaintedItem>
#include <QQmlEngine>
#include <QPainter>

#include <utility>

#include "../../utils/image.hpp"

#include "../scripts/process.hpp"

#include "../../macros.h"

namespace core::gui::scripts {
    class process;
}

namespace core::gui::components {
    class editor;
}

namespace core::gui::components::editor_elements {
  class element: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString status WRITE status READ status)

    friend class gui::components::editor;

   public:
    enum statuses {
      ACTIVE,
      HOVER,
      DEFAULT
    };

   public:
    Q_INVOKABLE
    QString
      status(
        _p(status, QString const&) = ""
      );

   private:
    _p(_status, gui::components::editor_elements::element::statuses) = DEFAULT;
  };

  class segment;
  class polyline;

  class point: public element {
    Q_OBJECT
    Q_PROPERTY(double x MEMBER x)
    Q_PROPERTY(double y MEMBER y)

   public:
    point(
      _p(x, double),
      _p(y, double)
    );

    point(
      _p(other, utils::algebra::point)
    ): point(other.x, other.y) {
    }

    point(
      _p(other, gui::components::editor_elements::point const&)
    ): point(other.x, other.y) {
    }

    operator
      utils::algebra::point(
      ) const {
        return {x, y};
      }

    gui::components::editor_elements::point &
    operator
      =(
        _p(other, gui::components::editor_elements::point)
      ) {
        x = other.x;
        y = other.y;

        return *this;
      }

    gui::components::editor_elements::point
    operator
      +(
        _p(other, point const&)
      ) const {
        return {x + other.x, y + other.y};
      }

    gui::components::editor_elements::point
    operator
      /(
        _p(ratio, double)
      ) const {
        return {x / ratio, y / ratio};
      }

   public:
    _p(radius, static inline int) = 10;
   public:
    _p(connected, QList<gui::components::editor_elements::segment *>);

    _p(x, double);
    _p(y, double);
  };

  class segment: public element {
    Q_OBJECT
    Q_PROPERTY(core::gui::components::editor_elements::point * s MEMBER s)
    Q_PROPERTY(core::gui::components::editor_elements::point * e MEMBER e)

   public:
    segment(
      _p(s, gui::components::editor_elements::point &),
      _p(e, gui::components::editor_elements::point &),
      //
      _p(polyline, gui::components::editor_elements::polyline *) = nullptr
    );

    segment(
      _p(other, gui::components::editor_elements::segment &)
    ): segment(*other.s, *other.e, other.polyline) {
    }

    ~segment() override;

    operator
      utils::algebra::segment (
      ) const {
        return {*s, *e};
      }

    Q_INVOKABLE
    void
      set_s(
        _p(point, gui::components::editor_elements::point *)
      );
    Q_INVOKABLE
    void
      set_e(
        _p(point, gui::components::editor_elements::point *)
      );

   public:
    _p(polyline, gui::components::editor_elements::polyline *);
    //
    _p(s, gui::components::editor_elements::point *);
    _p(e, gui::components::editor_elements::point *);
  };

  class text: public element {
    Q_OBJECT
    Q_PROPERTY(int     x       MEMBER x)
    Q_PROPERTY(int     y       MEMBER y)
    Q_PROPERTY(int     w       MEMBER w)
    Q_PROPERTY(int     h       MEMBER h)
    Q_PROPERTY(QString content MEMBER content)
    Q_PROPERTY(QString family  MEMBER family)
    Q_PROPERTY(int     size    MEMBER size)
    Q_PROPERTY(bool    bold    MEMBER bold)
    Q_PROPERTY(bool    italic  MEMBER italic)

   public:
    text(
      _p(x      , int),
      _p(y      , int),
      _p(w      , int),
      _p(h      , int),
      _p(content, QString),
      _p(family , QString),
      _p(size   , int),
      _p(bold   , bool),
      _p(italic , bool)
    ): x      (x),
       y      (y),
       w      (w),
       h      (h),
       content(std::move(content)),
       family (std::move(family)),
       size   (size),
       bold   (bold),
       italic (italic) {
    }

   public:
    _p(x      , int);
    _p(y      , int);
    _p(w      , int);
    _p(h      , int);
    _p(content, QString);
    _p(family , QString);
    _p(size   , int);
    _p(bold   , bool);
    _p(italic , bool);
  };

  class polyline: public element {
    Q_OBJECT
    Q_PROPERTY(bool closed MEMBER closed)
    Q_PROPERTY(QList<core::gui::components::editor_elements::segment *> segments MEMBER segments)

   public:
    explicit
    polyline(
      _p(closed, bool) = false
    ): closed(closed) {
    }

    polyline(
      _p(other, gui::components::editor_elements::polyline const&)
    ): segments(other.segments), closed(other.closed) {
    }

    polyline(
      _p(begin , QList<core::gui::components::editor_elements::segment *>::iterator const&),
      _p(end   , QList<core::gui::components::editor_elements::segment *>::iterator const&),
      _p(closed, bool) = false
    ): segments(QList<core::gui::components::editor_elements::segment *>(begin, end)), closed(closed) {
    }

   public:
    _p(closed, bool) = false;

    _p(segments, QList<core::gui::components::editor_elements::segment *>);
  };
}

Q_DECLARE_METATYPE(core::gui::components::editor_elements::point *)
Q_DECLARE_METATYPE(core::gui::components::editor_elements::segment *)
Q_DECLARE_METATYPE(core::gui::components::editor_elements::text *)
Q_DECLARE_METATYPE(core::gui::components::editor_elements::polyline *)

namespace core::gui::components {
  class editor: public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(core::gui::scripts::process * process MEMBER _process NOTIFY processChanged)

    Q_PROPERTY(float  zoom MEMBER zoom NOTIFY zoomChanged);
    Q_PROPERTY(QPoint pan  MEMBER pan  NOTIFY panChanged);

    Q_PROPERTY(QList<core::gui::components::editor_elements::point *> points MEMBER _points)
    Q_PROPERTY(QList<core::gui::components::editor_elements::polyline *> polylines MEMBER _polylines)

    friend class gui::scripts::process;

   public:
    void
      set_image(
        _p(image, core::utils::image::image *)
      );

    Q_INVOKABLE
    void
      set_process(
        _p(editor, core::gui::scripts::process *)
      );

    Q_INVOKABLE
    void
      home();

    Q_INVOKABLE
    core::gui::components::editor_elements::point *
      add_point(
        _p(x, double),
        _p(y, double)
      );

    Q_INVOKABLE
    core::gui::components::editor_elements::segment *
      add_segment(
        _p(s, core::gui::components::editor_elements::point *),
        _p(e, core::gui::components::editor_elements::point *)
      );

    Q_INVOKABLE
    core::gui::components::editor_elements::polyline *
      add_polyline(
        _p(contour, utils::polygon::polygon const&),
        _p(closed , bool) = false
      );

    Q_INVOKABLE
    void
      add_text(
        _p(x      , int),
        _p(y      , int),
        _p(w      , int),
        _p(h      , int),
        _p(content, QString const&),
        _p(family , QString const&),
        _p(size   , int),
        _p(bold   , bool),
        _p(italic , bool)
      );

    Q_INVOKABLE
    void
      remove_point(
        _p(point, core::gui::components::editor_elements::point *)
      );

    Q_INVOKABLE
    void
      remove_segment(
        _p(segment, core::gui::components::editor_elements::segment *)
      );

    Q_INVOKABLE
    void
      dissolve_point(
        _p(point, core::gui::components::editor_elements::point *)
      );

    Q_INVOKABLE
    void
      dissolve_segment(
        _p(segment, core::gui::components::editor_elements::segment *)
      );

    Q_INVOKABLE
    void
      simplify(
        _p(threshold_a, double),
        _p(threshold_b, double),
        _p(iterations , int),
        _p(polylines, QList<core::gui::components::editor_elements::polyline *>) = {}
      );

    Q_INVOKABLE
    void
      export_dxf(
        _p(path, QString const&),
        _p(rx  , double),
        _p(ry  , double)
      ) const;

    void
      paint(
        _p(painter, QPainter *)
      ) override;

   signals:
    void    zoomChanged();
    void     panChanged();
    void processChanged();

   public:
    _p(zoom, float);
    _p(pan , QPoint);

   private:
    _p(_process, gui::scripts::process *) = nullptr;
    //
    _p(_image, struct {
      _p(main, std::unique_ptr<QPixmap>);
      _p(temp, std::unique_ptr<QPixmap>);
      _p(zoom, float);
    });
    //
    _p(_points   , QList<gui::components::editor_elements::point *>);
    _p(_polylines, QList<gui::components::editor_elements::polyline *>);
    //
    _p(_texts, QList<gui::components::editor_elements::text *>);
  };
}

Q_DECLARE_METATYPE(core::gui::components::editor *)
