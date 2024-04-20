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

//
  core::gui::components::editor_elements::point::point(
    _p(x, double),
    _p(y, double)
  ): x(x), y(y) {
  }

//
  core::gui::components::editor_elements::segment::segment(
    _p(s, core::gui::components::editor_elements::point &),
    _p(e, core::gui::components::editor_elements::point &),
    //
    _p(polyline, core::gui::components::editor_elements::polyline *)
  ): s(&s), e(&e), polyline(polyline) {
    this->s->connected.push_back(this);
    this->e->connected.push_back(this);
  }
//
  core::gui::components::editor_elements::segment::~segment(
  ) {
    this->s->connected.removeOne(this);
    this->e->connected.removeOne(this);
  }

void
  core::gui::components::editor_elements::segment::set_s(
    _p(point, gui::components::editor_elements::point *)
  ) {
    this->s->connected.removeOne(this);
    s = point;
    this->s->connected.push_back(this);
  }
void
  core::gui::components::editor_elements::segment::set_e(
    _p(point, gui::components::editor_elements::point *)
  ) {
    this->e->connected.removeOne(this);
    e = point;
    this->e->connected.push_back(this);
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

core::gui::components::editor_elements::point *
  core::gui::components::editor::add_point(
    _p(x, double),
    _p(y, double)
  ) {
    auto element = new gui::components
      ::editor_elements::point(
        *new utils::algebra::point{x, y}
      );

    QQmlEngine::setObjectOwnership(element, QQmlEngine::CppOwnership);
    _points.push_back(element);

    return element;
  }

core::gui::components::editor_elements::segment *
  core::gui::components::editor::add_segment(
    _p(s, core::gui::components::editor_elements::point *),
    _p(e, core::gui::components::editor_elements::point *)
  ) {
    _p(polyline, core::gui::components::editor_elements::polyline *) = nullptr;
    for (auto const& segment : s->connected) {
      if (*segment->polyline->segments.end() == segment) {
        if (segment->e == s) {
          polyline = segment->polyline;
        } else if (segment->e == e) {
          auto * t = &s;
          s = e;
          e = *t;
        }
        break;
      }
    }

    if (not polyline) {
      polyline = new core::gui::components::editor_elements::polyline();
      _polylines.push_back(polyline);
    }

    auto element = new core::gui::components
      ::editor_elements::segment(
        *s,
        *e,
        polyline
      );

    QQmlEngine::setObjectOwnership(element, QQmlEngine::CppOwnership);
    polyline->segments.push_back(element);

    return element;
  }

core::gui::components::editor_elements::polyline *
  core::gui::components::editor::add_polyline(
    _p(contour, utils::polygon::polygon const&),
    _p(closed , bool)
  ) {
    _polylines.push_back(new gui::components::editor_elements::polyline());
    auto & element = _polylines.back();
    element->closed = closed;

    gui::components::editor_elements::point * start = nullptr, * last = nullptr;
    for (auto point = contour.begin(); point < contour.end(); ++point) {
      auto current = new gui::components::editor_elements::point(*point);
      if (not start)
        start = current;

      _points.push_back(current);
      if (last)
        element->segments.push_back(new gui::components::editor_elements::segment{*last, *current, element});

      last = current;
    }
    if (closed)
      element->segments.push_back(new gui::components::editor_elements::segment{*last, *start, element});

    return element;
  }

void
  core::gui::components::editor::add_text(
    _p(x      , int),
    _p(y      , int),
    _p(w      , int),
    _p(h      , int),
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
        content,
        family,
        size,
        bold,
        italic
      );

    QQmlEngine::setObjectOwnership(element, QQmlEngine::CppOwnership);
    _texts.push_back(element);
  }

void
  core::gui::components::editor::remove_point(
    _p(point, core::gui::components::editor_elements::point *)
  ) {
    for (auto const& segment : point->connected)
      remove_segment(segment);

    _points.removeOne(point);
  }

void
  core::gui::components::editor::remove_segment(
    _p(segment, core::gui::components::editor_elements::segment *)
  ) {
    auto polyline = segment->polyline;
    auto index    = polyline->segments.indexOf(segment);
    //
    delete segment;

    auto first = new core::gui::components::editor_elements::polyline(polyline->segments.begin(), polyline->segments.begin() + index);
    auto last  = new core::gui::components::editor_elements::polyline(polyline->segments.begin() + index + 1, polyline->segments.end());

    for (auto & s : first->segments)
      s->polyline = first;
    for (auto & s : last->segments)
      s->polyline = last;

    _polylines.removeOne(polyline);
    delete polyline;
    if (not first->segments.empty())
      _polylines.push_back(first);
    if (not last->segments.empty())
      _polylines.push_back(last);
  }

void
  core::gui::components::editor::dissolve_point(
    _p(point, core::gui::components::editor_elements::point *)
  ) {
    for (auto const& segment : point->connected) {
      auto polyline = segment->polyline;

      core::gui::components::editor_elements::segment * next = nullptr;
      for (auto const &other : point->connected) {
        if (other != segment and other->polyline == polyline) {
          next = other;
          break;
        }
      }

      if (next) {
        if (segment->e == next->s) {
          segment->set_e(next->e);
          remove_segment(next);
        } else {
          next->set_s(segment->s);
          remove_segment(segment);
        }
      }
    }

    _points.removeOne(point);
  }

void
  core::gui::components::editor::dissolve_segment(
    _p(segment, core::gui::components::editor_elements::segment *)
  ) {
    remove_segment(segment);
  }

core::utils::algebra::point
  close_point(
    _p(points, std::vector<core::utils::algebra::point> const&),
    _p(check, core::utils::algebra::point const&)
  );

QList<core::gui::components::editor_elements::segment *>
  simplify_midline(
    _p(polyline , QList<core::gui::components::editor_elements::segment *> const&),
    _p(threshold, double)
  ) {
    using namespace core;

    _p(lines, std::vector<std::pair<utils::algebra::line_eq, gui::components::editor_elements::segment *>>);
    //
    for (auto cursor = polyline.begin(); cursor < polyline.end();) {
      auto segment = *cursor++;

      auto line = core::utils::algebra::linear_regression({
        *segment->s,
        *segment->e,
      });

      for (; cursor < polyline.end() - 1; ++cursor) {
        auto segment_temp = *cursor;
        if (segment_temp->s->connected.size() > 2)
          break;

        line.add_point(*segment_temp->e);
        if (utils::algebra::distance::point_line(*segment_temp->e, line) >= threshold
          or utils::algebra::distance::point_line(*segment_temp->s, line) >= threshold) {
          line.sub_point(*segment_temp->e);

          break;
        }
      }

      if (cursor < polyline.end())
        segment->set_e((*cursor)->s);

      lines.emplace_back(line, segment);
    }

    _p(result, QList<core::gui::components::editor_elements::segment *>);

    for (auto const& pair : lines)
      result.push_back(new gui::components::editor_elements::segment(*pair.second));

//    for (auto pair = lines.begin(); pair < lines.end(); ++pair) {
//      auto last_pair = pair - 1 >= lines.begin() ? pair - 1 : lines.end() - 1;
//      auto next_pair = pair + 1 <  lines.end()   ? pair + 1 : lines.begin();
//
//      auto line = pair->first;
//      auto segment = pair->second;
//
//      auto s = close_point({line.y(segment->s->x), line.x(segment->s->y)}, *segment->s);
//      auto e = close_point({line.y(segment->e->x), line.x(segment->e->y)}, *segment->e);
//
//      auto st = utils::algebra::intercept::line_line(last_pair->first, pair->first);
//      auto et = utils::algebra::intercept::line_line(next_pair->first, pair->first);
//
//      *segment->s = utils::algebra::distance::point_point(st, s) < threshold ? st : s;
//      *segment->e = utils::algebra::distance::point_point(et, e) < threshold ? et : e;
//
//      result.push_back(new gui::components::editor_elements::segment(*segment));
//    }

    return result;
  }

QList<core::gui::components::editor_elements::segment *>
  simplify_inline(
    _p(polyline , QList<core::gui::components::editor_elements::segment *> const&),
    _p(threshold, double)
  ) {
    using namespace core;

    _p(result, QList<core::gui::components::editor_elements::segment *>);
    //
    for (auto cursor = polyline.begin(); cursor < polyline.end();) {
      auto segment = *cursor++;

      for (; cursor < polyline.end() - 1; ++cursor) {
        auto segment_temp = *cursor;
        if (segment_temp->s->connected.size() > 2)
          break;

        if (utils::algebra::distance::point_segment(*segment_temp->e, *segment) >= threshold) {
          break;
        }

        segment->set_e(segment_temp->e);
      }

      result.push_back(new gui::components::editor_elements::segment(*segment));
    }

    return result;
  }

void
  core::gui::components::editor::simplify(
    _p(threshold_a, double),
    _p(threshold_b, double),
    _p(iterations , int),
    _p(polylines, QList<core::gui::components::editor_elements::polyline *>)
  ) {
    if (polylines.empty())
      polylines = _polylines;

    for (int _ = 0; _ < iterations; ++_) {
      for (auto &polyline: polylines) {
        auto simplified_1 = simplify_midline(polyline->segments, threshold_a);
        qDeleteAll(polyline->segments);
        polyline->segments.clear();
        polyline->segments.append(simplified_1);
        auto simplified_2 = simplify_inline(polyline->segments, threshold_b);
        qDeleteAll(polyline->segments);
        polyline->segments.clear();
        polyline->segments.append(simplified_2);
      }
    }

    for (auto point = _points.begin(); point < _points.end(); ++point)
      if ((*point)->connected.empty())
        _points.erase(point--);
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

    for (auto const& polyline : _polylines) {
      for (auto const& segment : polyline->segments) {
        QColor color;
        switch (segment->_status) {
          case gui::components::editor_elements::element::statuses::DEFAULT:
            color.setRgb(55, 173, 118);
            break;
          case gui::components::editor_elements::element::statuses::ACTIVE:
            color.setRgb(73, 33, 35);
            break;
          case gui::components::editor_elements::element::statuses::HOVER:
            color.setRgb(7, 56, 92);
            break;
          default:;
        }
        painter->setPen(QPen(color, 4));

        painter->drawLine(
          QPoint{int(segment->s->x * zoom + pan.x()), int(segment->s->y * zoom + pan.y())},
          QPoint{int(segment->e->x * zoom + pan.x()), int(segment->e->y * zoom + pan.y())}
        );
      }
    }

    const int radius = gui::components::editor_elements::point::radius;
    //
    painter->setPen(QPen({}, 0));
    for (auto const& point : _points) {
      QColor color;
      switch (point->_status) {
        case gui::components::editor_elements::element::statuses::DEFAULT:
          color.setRgb(16, 138, 227);
          break;
        case gui::components::editor_elements::element::statuses::ACTIVE:
          color.setRgb(73, 33, 35);
          break;
        case gui::components::editor_elements::element::statuses::HOVER:
          color.setRgb(7, 56, 92);
          break;
        default:;
      }
      painter->setBrush(QBrush(color));

      painter->drawEllipse(
        int(point->x * zoom + pan.x() - radius / 2.),
        int(point->y * zoom + pan.y() - radius / 2.),
        radius, radius
      );
    }

    painter->setPen(QPen(QColor(0, 255, 100), 4));
    for (auto const &text: _texts) {
      QFont font;
      font.setFamily   (text->family);
      font.setPixelSize(text->size * zoom);
      font.setBold     (text->bold);
      font.setItalic   (text->italic);
      //
      painter->setFont(font);
      painter->drawText(
        int(double(text->x) * zoom + pan.x()),
        int(double(text->y) * zoom + pan.y()),
        int(double(text->w) * zoom),
        int(double(text->h) * zoom),
        0,
        text->content
      );
    }
  }

void
  core::gui::components::editor::export_dxf(
    _p(path, QString const&),
    _p(rx  , double),
    _p(ry  , double)
  ) const {
    DL_Dxf dxf;

    auto writer = dxf.out(path.toStdString().c_str(), DL_Codes::AC1009);
    if (writer == nullptr)
      return;

    dxf.writeHeader(*writer);
    writer->dxfString(9, "$INSUNITS");
    writer->dxfInt(70, 5);
    writer->dxfString(9, "$DIMEXE");
    writer->dxfReal(40, 1.25);
    writer->dxfString(9, "$TEXTSTYLE");
    writer->dxfString(7, "Standard");
    // vector variable:
    writer->dxfString(9, "$LIMMIN");
    writer->dxfReal(10, 0.0);
    writer->dxfReal(20, 0.0);
    writer->sectionEnd();

    writer->sectionTables();
    dxf.writeVPort(*writer);
    writer->tableLineTypes(3);
    dxf.writeLineType(*writer, DL_LineTypeData("BYBLOCK", 0));
    dxf.writeLineType(*writer, DL_LineTypeData("BYLAYER", 0));
    dxf.writeLineType(*writer, DL_LineTypeData("CONTINUOUS", 0));
    writer->tableEnd();

    writer->tableLayers(1);
    dxf.writeLayer(
            *writer,
            DL_LayerData("0", 0),
            DL_Attributes("", DL_Codes::black, 100, "CONTINUOUS"));
    writer->tableEnd();

    dxf.writeStyle(*writer);
    dxf.writeView(*writer);
    dxf.writeUcs(*writer);
    writer->tableAppid(1);
    writer->tableAppidEntry(0x12);
    writer->dxfString(2, "ACAD");
    writer->dxfInt(70, 0);
    writer->tableEnd();

    dxf.writeBlockRecord(*writer);
    writer->tableEnd();
    writer->sectionEnd();

    writer->sectionBlocks();
    writer->sectionEnd();

    writer->sectionEntities();
    for (auto const& polyline : _polylines) {
      for (auto const& segment : polyline->segments) {
        dxf.writeLine(
          *writer,
          DL_LineData(
            segment->s->x * rx, segment->s->y * ry, 0,
            segment->e->x * rx, segment->e->y * ry, 0
          ),
          DL_Attributes("", 256, -1, "BYLAYER")
        );
      }
    }

    for (auto const& text: _texts) {
      qDebug() << text->x * rx << text->y * ry << text->w * std::abs(rx) << text->h *std::abs(ry) << text->size * std::abs(ry);
      dxf.writeText(
        *writer,
        DL_TextData(
          text->x * rx,
          text->y * ry,
          0,
          0,
          0,
          0,
          5, 1,
          0, 0, 0,
          text->content.toStdString(),
          std::to_string(int(std::ceil(text->size * std::abs(ry)))) + "px arial",
          0
        ),
        DL_Attributes("", 256, -1, "BYLAYER")
      );
    }
    writer->sectionEnd();

    dxf.writeObjects(*writer);
    dxf.writeObjectsEnd(*writer);

    writer->dxfEOF();
    writer->close();

    delete writer;
  }