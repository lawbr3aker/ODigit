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

  namespace editor_elements {
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

    class point: public element {
      Q_OBJECT
      Q_PROPERTY(double x MEMBER x)
      Q_PROPERTY(double y MEMBER y)

     public:
      explicit point(
        _p(x, double),
        _p(y, double)
      ): x(x), y(y) {
      }

      point(
        _p(other, utils::algebra::point)
      ): x(other.x), y(other.y) {
      }

      point(
        _p(other, gui::components::editor_elements::point const&)
      ): x(other.x), y(other.y) {
      }

     public:
      _p(x, double);
      _p(y, double);
    };

    class line: public element {
      Q_OBJECT
      Q_PROPERTY(core::gui::components::editor_elements::point * s MEMBER s)
      Q_PROPERTY(core::gui::components::editor_elements::point * e MEMBER e)

     public:
      line(
        _p(s, gui::components::editor_elements::point &),
        _p(e, gui::components::editor_elements::point &)
      ): s(&s), e(&e) {}

      line(
        _p(other, gui::components::editor_elements::line const&)
      ): s(other.s), e(other.s) {}

     public:
      _p(s, gui::components::editor_elements::point *);
      _p(e, gui::components::editor_elements::point *);
    };

    class text: public QObject {
      Q_OBJECT
      Q_PROPERTY(int     x       MEMBER x)
      Q_PROPERTY(int     y       MEMBER y)
      Q_PROPERTY(int     w       MEMBER w)
      Q_PROPERTY(int     h       MEMBER h)
      Q_PROPERTY(float   scale   MEMBER scale)
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
        _p(scale  , float),
        _p(content, QString),
        _p(family , QString),
        _p(size   , int),
        _p(bold   , bool),
        _p(italic , bool)
      ): x      (x),
         y      (y),
         w      (w),
         h      (h),
         scale  (scale),
         content(std::move(content)),
         family (std::move(family)),
         size   (size),
         bold   (bold),
         italic (italic) {}

     public:
      _p(x      , int);
      _p(y      , int);
      _p(w      , int);
      _p(h      , int);
      _p(scale  , float);
      _p(content, QString);
      _p(family , QString);
      _p(size   , int);
      _p(bold   , bool);
      _p(italic , bool);
    };

    class shape: public QObject {
     public:
      shape(
        _p(polygon, std::vector<core::utils::algebra::point> &),
        _p(closed , bool) = true
      );

     public:
      _p(points   , std::vector<core::gui::components::editor_elements::point *>);
      _p(polylines, std::vector<std::vector<core::gui::components::editor_elements::line *>>);
    };
  }

  class editor: public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(core::gui::scripts::process * process MEMBER _process NOTIFY processChanged)

    Q_PROPERTY(float  zoom MEMBER zoom NOTIFY zoomChanged);
    Q_PROPERTY(QPoint pan  MEMBER pan  NOTIFY panChanged);

    Q_PROPERTY(QList<core::gui::components::editor_elements::point *> points MEMBER _points)
    Q_PROPERTY(QList<core::gui::components::editor_elements::line *> lines MEMBER _lines)

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
    void
      update_elements();

    Q_INVOKABLE
    core::gui::components::editor_elements::point *
      add_point(
        _p(x, double),
        _p(y, double)
      );

    Q_INVOKABLE
    core::gui::components::editor_elements::line *
      add_line(
        _p(s, core::gui::components::editor_elements::point *),
        _p(e, core::gui::components::editor_elements::point *)
      );

    Q_INVOKABLE
    core::gui::components::editor_elements::shape *
      add_shape(
        _p(shape, core::gui::components::editor_elements::shape *)
      );

    Q_INVOKABLE
    core::gui::components::editor_elements::text *
      add_text(
        _p(x      , int),
        _p(y      , int),
        _p(w      , int),
        _p(h      , int),
        _p(scale  , float),
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
      remove_line(
        _p(line, core::gui::components::editor_elements::line *)
      );

    Q_INVOKABLE
    void
      simplify(
        _p(threshold_a, double),
        _p(threshold_b, double),
        _p(polylines, std::vector<std::vector<core::gui::components::editor_elements::line *>>) = {}
      );

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
    _p(_shapes, QList<gui::components::editor_elements::shape *>);
    _p(_points, QList<gui::components::editor_elements::point *>);
    _p(_lines , QList<gui::components::editor_elements::line *>);
    _p(_texts , QList<gui::components::editor_elements::text *>);
  };
}


Q_DECLARE_METATYPE(core::gui::components::editor_elements::point *)
Q_DECLARE_METATYPE(core::gui::components::editor_elements::line *)
Q_DECLARE_METATYPE(core::gui::components::editor_elements::text *)
Q_DECLARE_METATYPE(core::gui::components::editor_elements::shape *)

Q_DECLARE_METATYPE(core::gui::components::editor *)
