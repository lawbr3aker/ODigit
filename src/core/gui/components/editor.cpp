#include "./editor.hpp"

QString
  core::gui::components::editor_elements::element::status(
    _p(status, QString const&)
  ) {
    if (status != "") {
      if (status == "active")
        _status = element::statuses::ACTIVE;
      else if (status == "hover")
        _status = element::statuses::HOVER;
      else
        _status = element::statuses::DEFAULT;
    }

    switch (_status) {
      case element::statuses::ACTIVE:
        return "active";
      case element::statuses::HOVER:
        return "hover";
      default:
        return "default";
    }
  }

void
  core::gui::components::editor::set_image(
    _p(image, core::utils::image::image *)
  ) {
    cv::Mat temp;
    cvtColor(*image, temp,cv::COLOR_BGR2RGB);
    QImage final((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    final.bits();

    _image.main = std::make_unique<QPixmap>(QPixmap::fromImage(final));
    _image.temp.reset();
  }

void
  core::gui::components::editor::set_process(
    _p(process, core::gui::scripts::process *)
  ) {
    _process = process;
  }

void
  core::gui::components::editor::home(
  ) {
    if (_image.main) {
      double width = this->width(), height = this->height();

      if (width > height)
        zoom = (float) (height / _image.main->height());
      else
        zoom = (float) (width / _image.main->width());
    }
    pan.setX(0);
    pan.setY(0);

    emit zoomChanged();
    emit panChanged();
  }

void
  core::gui::components::editor::update_elements(
  ) {
    qDeleteAll(_points.begin(), _points.end()); _points.clear();
    qDeleteAll(_lines.begin(), _lines.end()); _lines.clear();

    int offset = 0;
    for (auto const& contour : _process->_contours_final) {
      {
        auto const& point = contour[0];

        _points.push_back(new gui::components::editor_elements::point{point.x, point.y});
      }
      for (int i = 1; i < contour.size(); ++i) {
        auto const& point = contour[i];

        _points.push_back(new gui::components::editor_elements::point{point.x, point.y});
        _lines.push_back(new gui::components::editor_elements::line{_points[offset + i - 1], _points[offset + i]});
      }
      _lines.push_back(new gui::components::editor_elements::line{_points[offset + contour.size() - 1], _points[offset]});

      offset += contour.size();
    }
  }

core::gui::components::editor_elements::point *
  core::gui::components::editor::add_point(
    _p(x, double),
    _p(y, double)
  ) {
    auto element = new core::gui::components
      ::editor_elements::point(
        x,
        y
      );

    QQmlEngine::setObjectOwnership(element, QQmlEngine::CppOwnership);
    _points.push_back(element);

    return element;
  }

core::gui::components::editor_elements::line *
  core::gui::components::editor::add_line(
    _p(s, core::gui::components::editor_elements::point *),
    _p(e, core::gui::components::editor_elements::point *)
  ) {
    auto element = new core::gui::components
      ::editor_elements::line(
        s,
        e
      );

    QQmlEngine::setObjectOwnership(element, QQmlEngine::CppOwnership);
    _lines.push_back(element);

    return element;
  }

core::gui::components::editor_elements::text *
  core::gui::components::editor::add_text(
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
  ) {
    auto element = new core::gui::components
      ::editor_elements::text(
        x,
        y,
        w,
        h,
        scale,
        content,
        family,
        size,
        bold,
        italic
      );

    QQmlEngine::setObjectOwnership(element, QQmlEngine::CppOwnership);
    _texts.push_back(element);

    return element;
  }

void
  core::gui::components::editor::remove_point(
    _p(point, core::gui::components::editor_elements::point *)
  ) {
    _points.erase(std::find(_points.begin(), _points.end(), point));
  }

void
  core::gui::components::editor::remove_line(
    _p(line, core::gui::components::editor_elements::line *)
  ) {
    _lines.erase(std::find(_lines.begin(), _lines.end(), line));
  }

void
  core::gui::components::editor::paint(
    _p(painter, QPainter *)
  ) {
    using namespace core;

    if (not _process->active)
      return;

    if (_image.main) {
      if (not _image.temp or zoom != _image.zoom) {
        _image.temp = std::make_unique<QPixmap>(
          _image.main->scaled(
            _image.main->size() * zoom,
            Qt::KeepAspectRatio, Qt::FastTransformation
          )
        );
        _image.zoom = zoom;
      }

      painter->drawPixmap(pan, *_image.temp);
    }

    painter->setPen(QPen(QColor(55, 173, 118), 4));
    for (auto const& line: _lines) {
      painter->drawLine(
        QPoint{int(line->s->x * zoom + pan.x()), int(line->s->y * zoom + pan.y())},
        QPoint{int(line->e->x * zoom + pan.x()), int(line->e->y * zoom + pan.y())}
      );
    }

    const int r = 12;
    for (auto state: {
      gui::components::editor_elements::element::statuses::DEFAULT,
      gui::components::editor_elements::element::statuses::ACTIVE,
      gui::components::editor_elements::element::statuses::HOVER
    }) {
      switch (state) {
        case gui::components::editor_elements::element::statuses::DEFAULT:
          painter->setBrush(QColor(16, 138, 227));
          break;
        case gui::components::editor_elements::element::statuses::ACTIVE:
          painter->setBrush(QColor(73, 33, 35));
          break;
        case gui::components::editor_elements::element::statuses::HOVER:
          painter->setBrush(QColor(7, 56, 92));
          break;
        default:;
      }
      for (auto const &point: _points) {
        if (point->_status == state) {
          const double x = point->x * zoom + pan.x();
          const double y = point->y * zoom + pan.y();

          painter->drawEllipse(int(x - r / 2.), int(y - r / 2.), r, r);
        }
      }
    }

    for (auto const &text: _texts) {
      QFont font;
      font.setFamily(text->family);
      double s = zoom / text->scale;
      font.setPixelSize(s * text->size);
      font.setBold(text->bold);
      font.setItalic(text->italic);
      painter->setFont(font);
      painter->drawText((int) (s * text->x + pan.x()), (int) (s * text->y + pan.y()), (int) (s * text->w),
                        (int) (s * text->h), 0, text->content);
    }
  }
