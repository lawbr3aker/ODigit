#include <windows.h>

#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QSplashScreen>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QStyleFactory>
#include <QDebug>
#include <QStandardPaths>
#include <QSurfaceFormat>
#include <QQuickView>
#include <QTextCodec>

#include "core/gui/components/ruler.hpp"
#include "core/gui/components/editor.hpp"

#include "core/gui/scripts/process.hpp"
#include "core/gui/scripts/config.hpp"
#include "core/gui/scripts/translator.hpp"

#include "psimpl/psimpl.h"

void shimage(cv::Mat const& image, int width = 1200) {
  cv::Mat scaled;
  cv::Size size{width, int((float) width / image.cols * image.rows)};
  cv::resize(image, scaled, size);
  cv::imshow("output.jpg", scaled);
//  cv::imwrite("temp.jpg", image);
  cv::waitKey(0);
}

// #define SPLASH

core::utils::algebra::line_eq
  linear_simplify(
    _p(cursor, core::utils::polygon::polygon::const_iterator &),
    _p(end   , core::utils::polygon::polygon::const_iterator const&),
    //
    _p(threshold, double)
  );
cv::Mat image;

core::utils::polygon::polygon
  simplify(
    _p(polygon  , core::utils::polygon::polygon const&),
    _p(threshold, double) = 5
  ) {
    using namespace core;

    std::vector<utils::algebra::line_eq> lines;
    std::vector<utils::algebra::line> segments;

    for (auto cursor = polygon.begin(); cursor < polygon.end();) {
      auto last = cursor;
      auto line = linear_simplify(cursor, polygon.end(), threshold);
      //
      if (last != cursor) {
        lines.push_back(line);
        segments.push_back({*last, *cursor});
      } else {
        lines.push_back(utils::algebra::linear_regression({*last, polygon[0]}));
        segments.push_back({*last, polygon[0]});
        break;
      }
    }

    utils::polygon::polygon final;

    double t = threshold * 3.14;

    auto checker = utils::algebra::linear_regression();
    auto checker_begin = utils::algebra::point();

    auto line    = lines.begin()   , last_line    = lines.end() - 1;
    auto segment = segments.begin(), last_segment = segments.end() - 1;
    for (; line < lines.end(); last_line = line++, last_segment = segment++) {
      auto point = utils::algebra::intercept::line_line(*last_line, *line);

      auto min_x = std::min({last_segment->s.x, last_segment->s.x, segment->s.x, segment->s.x});
      auto max_x = std::max({last_segment->s.x, last_segment->s.x, segment->s.x, segment->s.x});
      auto min_y = std::min({last_segment->s.y, last_segment->s.y, segment->s.y, segment->s.y});
      auto max_y = std::max({last_segment->s.y, last_segment->s.y, segment->s.y, segment->s.y});

      if (min_x - t <= point.x and point.x <= max_x + t
      and min_y - t <= point.y and point.y <= max_y + t) {
      } else {
        cv::circle(image, {int(point.x), int(point.y)}, 20, {0, 255, 0}, 10);
        point = line->y(std::abs(point.x - max_x) < std::abs(point.x - min_x) ? max_x : min_x);

        cv::circle(image, {int(point.x), int(point.y)}, 20, {255, 0, 0}, 10);
      }
      cv::circle(image, {int(point.x), int(point.y)}, 20, {0, 0, 255}, 10);
      final.push_back(point);

//      if (checker.slope == 0)
//        checker_begin = point;
//      auto temp = image.clone();
//      cv::line(temp, {0, int(checker.y(0))}, {temp.cols, int(checker.y(temp.cols))}, {0, 255, 0}, 6);
//      cv::circle(temp, {int(point.x), int(point.y)}, 20, {255, 255, 255}, 10);
//      const double d = 10;
//      std::cout << utils::algebra::point2line(point, checker) << " " << (utils::algebra::point2line(point, checker) > d) << '\n';
//      shimage(temp);
//      if (utils::algebra::point2line(point, checker) > d) {
//        checker = utils::algebra::linear_regression({point});
//        final.push_back(checker_begin);
//        cv::circle(image, {int(checker_begin.x), int(checker_begin.y)}, 20, {255, 0, 255}, 10);
//        checker_begin = point;
//      }
    }

    utils::polygon::polygon final2 = final;


//    auto past_point = final.end() - 2;
//    auto last_point = final.end() - 1;
//    for (auto point = final.begin(); point < final.end(); ++point) {
//      auto temp = image.clone();
//      std::cout << utils::algebra::point2line(*last_point, {*past_point, *point}) << '\n';
//      auto l = utils::algebra::linear_regression({*past_point, *point});
//      cv::line(temp, {0, int(l.y(0))}, {temp.cols, int(l.y(temp.cols))}, {0, 255, 0}, 6);
//      cv::circle(temp, {int(last_point->x), int(last_point->y)}, 20, {0, 255, 0}, 10);
//      shimage(temp);
//      if (utils::algebra::point2line(*last_point, {*past_point, *point}) > 7)
//        final2.push_back(*last_point);
//
//      past_point = last_point;
//      last_point = point;
//    }

    {
      utils::algebra::point const *last = nullptr;
      for (auto const& point : final2) {
        cv::circle(image, {int(point.x), int(point.y)}, 20, {255, 0, 255}, 10);
        if (last)
          cv::line(image, {int(last->x), int(last->y)}, {int(point.x), int(point.y)}, {0, 0, 255}, 6);
        last = &point;
      }
      cv::line(image, {int(last->x), int(last->y)}, {int(final2[0].x), int(final2[0].y)}, {0, 0, 255}, 6);
    }
//    shimage(image);

    return final;
  }

//#define DEB

core::utils::polygon::polygon
  s(
    _p(polygon, core::utils::polygon::polygon const&),
    //
    _p(threshold_angle , double),
    _p(threshold_length, double),
    _p(threshold_curve , double),
    bool deb = false
  ) {
    using namespace core;

    _p(result, std::unordered_map<int, std::tuple<utils::algebra::line_eq, utils::algebra::point>>);
    //
    {
      _p(cursor, auto) = polygon.begin();
      std::function<void(int, utils::algebra::linear_regression *, double)>
        reconstruct = [&](
          _p(index , int),
          _p(line  , utils::algebra::linear_regression *) = nullptr,
          _p(length, double) = 0
        ) {
          auto begin = *cursor;

          auto past_point = cursor++;
          auto last_point = cursor++;
          //
          auto points = utils::algebra::linear_regression({*past_point, *last_point});

          length += utils::algebra::point2point(*past_point, *last_point);

          if (deb) {
          cv::circle(image, *past_point, 15, {0, 0, 255}, 10);
          cv::circle(image, *last_point, 15, {0, 255, 0}, 10);
          qDebug() << "called\n" << "index:" << index << "| remained:" << polygon.end() - cursor;
          }

          for (auto & point = cursor; point < polygon.end(); ++point) {
            auto angle = utils::algebra::angle::vertices(*past_point, *last_point, *point);

            if (deb) {
            cv::circle(image, *last_point, 15, {0, 255, 0}, 10);
            auto temp = image.clone();
            cv::circle(temp, *last_point, 15, {255, 255, 255}, 10);
            cv::line(temp, *past_point, *last_point, {0, 255, 0}, 5);
            cv::line(temp, *last_point, *point, {0, 255, 0}, 5);
            cv::line(temp, points.y(0), points.y(image.cols), {0, 255, 0}, 5);
            qDebug()
            << "length:" << length << threshold_length
            << "| angle:" << angle * 180.f / M_PI;
            shimage(temp);
            }

            if (angle < threshold_angle) {
              if (length < threshold_length) {
                if (line)
                  line->merge(points);
                else
                  line = &points;
                //
                --cursor; reconstruct(index, line, length);
              } else {
                line = &points;
                //
                --cursor; reconstruct(++index, line, 0);
                result[index] = std::make_tuple(*line, begin);
              }
              return;

            } else {
              length += utils::algebra::point2point(*last_point, *point);
              points.add_point(*point);

              if (utils::algebra::point2line(*point, line and length < threshold_length ? (*line).merge(points) : points) >= threshold_curve) {
                line = &points;
                //
                result[++index] = std::make_tuple(*line, *point);
                --cursor; reconstruct(index, new utils::algebra::linear_regression(), 0);
                return;
              }
            }

            past_point = last_point;
            last_point = point;
          }
          line = &points;
          //
          result[++index] = std::make_tuple(*line, begin);
        };

      reconstruct(-1, new utils::algebra::linear_regression(), 0);
    }

    core::utils::polygon::polygon final;
    //


    if (deb) {
    for (int index = 0; index < result.size(); ++index) {
      auto point = std::get<1>(result[index]);
      auto line = std::get<0>(result[index]);
      auto temp = image.clone();

      cv::line(temp, line.y(0), line.y(temp.cols), {255, 255, 255}, 8);
      cv::circle(temp, point, 20, {0, 0, 255}, 15);
      shimage(temp);
    }
    }

    for (int index = 0; index < result.size(); ++index)
      final.push_back(std::get<1>(result[index]));

    return final;
  }

int main(int argc, char * argv[]) {
  QApplication app(argc, argv);
  //
  core::gui::scripts::process process;
  //
  core::gui::scripts::config config;
  config.load(R"(C:\Users\lawbr3aker\AppData\Roaming\Optitex Pattern Design\ODigit\config.json)", "");
  //
  core::gui::components::editor editor;
  editor.set_process(&process);
  //
  process.set_config(&config);
  process.set_editor(&editor);
  process.step_path(R"(E:\Qt\F\FabR\tests\W177.5 H101.JPG)");
  process.step_detect();
  process.step_process();

  auto const& polygons = process._contours;

  /*core::utils::polygon::polygon test{
    {300, 300}, {500, 500}, {700, 300}, {900, 500}, {1100, 300}, {1300, 500}, {1500, 300}
  };
  image = process._image->clone();
  for (auto const& point: test)
    cv::circle(image, point, 20, {0, 0, 255}, 15);
  auto result = s(test, 160. * (M_PI / 180.), 300, 1000, true);
  for (auto const& point: result)
    cv::circle(image, point, 20, {0, 255, 0}, 15);
  shimage(image);*/

  image = process._image->clone();
  for (const auto & polygon : polygons) {
    auto result = simplify(polygon, 5);
    qDebug() << "result:" << result.size();
    {
      auto temp = image.clone();
      auto last = result.end() - 1;
      for (auto point = result.begin(); point < result.end(); ++point) {
        cv::circle(temp, {int(point->x), int(point->y)}, 20, {0, 0, 255}, 15);

        cv::line(temp, *last, *point, {0, 255, 0}, 15);
        last = point;
      }
      shimage(temp);
    }
    {
      auto a = s(result, 160. * (M_PI / 180.), 80, 5, false);
      auto temp = image.clone();
      auto last = a.end() - 1;
      for (auto point = a.begin(); point < a.end(); ++point) {
        cv::circle(temp, {int(point->x), int(point->y)}, 20, {0, 0, 255}, 15);

        cv::line(temp, *last, *point, {0, 255, 0}, 15);
        last = point;
      }
      shimage(temp);
    }
  }
}
