#include "./polygon.hpp"

core::utils::polygon::polygon
  core::utils::polygon::simplify_rdp(
    _p(polygon   , core::utils::polygon::polygon const&),
    _p(min_height, float),
    _p(min_width , float) = 0
  ) {
    using namespace core;

    if (polygon.size() < 3)
      return polygon;

    int ps_index = 0;
    int pe_index = 0;
    //
    double max_width = 0;
    for (int ps = 0; ps < polygon.size(); ++ps)
      for (int pe = ps + 1; pe < polygon.size(); ++pe) {
        double distance = utils::algebra::point2point(polygon[ps], polygon[pe]);
        if (distance > max_width) {
          ps_index = ps;
          pe_index = pe;
          max_width = distance;
        }
      }

    if (max_width < min_width)
      return {polygon[ps_index], polygon[ps_index]};

    int    max_index = 0;
    double max_distance = 0;
    for (int i = 1; i < polygon.size() - 1; ++i) {
      double distance = utils::algebra::point2line(polygon[i], utils::algebra::line{polygon[ps_index], polygon[pe_index]});
      if (distance > max_distance) {
        max_index    = i;
        max_distance = distance;
      }
    }

    if (max_distance > min_height) {
      auto rec_1 = utils::polygon::polygon(polygon.begin()            , polygon.begin() + max_index + 1);
      auto rec_2 = utils::polygon::polygon(polygon.begin() + max_index, polygon.end());

      rec_1 = utils::polygon::simplify_rdp(rec_1, min_height, min_width);
      rec_2 = utils::polygon::simplify_rdp(rec_2, min_height, min_width);

      utils::polygon::polygon result;
      result.insert(result.end(), rec_1.begin(), rec_1.end() - 1);
      result.insert(result.end(), rec_2.begin(), rec_2.end());

      return result;
    } else
      return {polygon[ps_index], polygon[ps_index]};
  }

core::utils::algebra::line_eq
  linear_simplify(
    _p(cursor, core::utils::polygon::polygon::const_iterator &),
    _p(end   , core::utils::polygon::polygon::const_iterator const&),
    //
    _p(threshold, double)
  ) {
    using namespace core;

    utils::algebra::linear_regression line;
    for (int i = 0;;){
      if (cursor + i >= end)
        break;

      auto const& point = cursor + i++;
      //
      line.add_point(*point);
      if (i > 2 and utils::algebra::point2line(*point, line) > threshold) {
        cursor += i - 2;
        break;
      }
    }

    return {
      line.slope,
      line.offset
    };
  }

core::utils::polygon::polygon
  core::utils::polygon::simplify_midline(
    _p(polygon, core::utils::polygon::polygon const&),
    //
    _p(threshold_curve, double)
  ) {
    using namespace core;

    std::vector<utils::algebra::line_eq> lines;
    std::vector<utils::algebra::line> segments;

    for (auto cursor = polygon.begin(); cursor < polygon.end();) {
      auto last = cursor;
      auto line = linear_simplify(cursor, polygon.end(), threshold_curve);
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

    double t = threshold_curve * 3.14;
    //
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
      } else
        point = line->y(std::abs(point.x - max_x) < std::abs(point.x - min_x) ? max_x : min_x);

      final.push_back(point);
    }

    return final;
  }

core::utils::polygon::polygon
  core::utils::polygon::simplify_slope(
    _p(polygon, core::utils::polygon::polygon const&),
    //
    _p(threshold_angle , double),
    _p(threshold_length, double),
    _p(threshold_curve , double)
  ) {
  using namespace core;

  _p(result, std::unordered_map<int, std::tuple<utils::algebra::line_eq, utils::algebra::point>>);
  //
  {
    _p(cursor, auto) = polygon.begin();
    std::function<void(int, utils::algebra::linear_regression *)>
      reconstruct = [&](
        _p(index, int),
        _p(line , utils::algebra::linear_regression *) = nullptr
      ) {
        auto begin = *cursor;

        auto past_point = cursor++;
        auto last_point = cursor++;
        //
        auto length = utils::algebra::point2point(*past_point, *last_point);
        auto points = utils::algebra::linear_regression({*past_point, *last_point});

        for (auto & point = cursor; point < polygon.end(); ++point) {
          auto angle = utils::algebra::angle::vertices(*past_point, *last_point, *point);

          if (angle < threshold_angle) {
            if (length < threshold_length) {
              if (line)
                line->merge(points);
              else
                line = &points;
              //
              --cursor; reconstruct(index, line);
            } else {
              line = &points;
              //
              --cursor; reconstruct(++index, line);
              result[index] = std::make_tuple(*line, begin);
            }
            return;
          } else {
            length += utils::algebra::point2point(*last_point, *point);
            points.add_point(*point);

            if (utils::algebra::point2line(*point, points) > threshold_curve) {
              line = &points;
              //
              --cursor; reconstruct(++index, line);
              result[index] = std::make_tuple(*line, begin);
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

    reconstruct(-1, new utils::algebra::linear_regression());
  }

  core::utils::polygon::polygon final;

  for (int i = 0; i < result.size(); ++i)
    final.push_back(std::get<1>(result[i]));

  return final;
}