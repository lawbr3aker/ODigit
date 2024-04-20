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
        double distance = utils::algebra::distance::point_point(polygon[ps], polygon[pe]);
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
      double distance = utils::algebra::distance::point_segment(polygon[i], utils::algebra::segment{polygon[ps_index], polygon[pe_index]});
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

core::utils::algebra::point
  close_point(
    _p(points, std::vector<core::utils::algebra::point> const&),
    _p(check, core::utils::algebra::point const&)
  ) {
    using namespace core;

    _p(min_point, core::utils::algebra::point const*) = nullptr;
    _p(min_distance, double);
    for (auto const& point : points) {
      auto distance = utils::algebra::distance::point_point(point, check);
      if (min_point == nullptr or distance < min_distance) {
        min_point = &point;
        min_distance = distance;
      }
    }

    return *min_point;
  }

core::utils::polygon::polygon
  core::utils::polygon::simplify_midline(
    _p(polygon  , core::utils::polygon::polygon const&),
    _p(threshold, double)
  ) {
    using namespace core;

    _p(lines, std::vector<std::pair<utils::algebra::line_eq, utils::algebra::segment>>);
    //
    for (auto cursor = polygon.begin(); cursor < polygon.end();) {
      auto s = *cursor;

      auto line = core::utils::algebra::linear_regression({
        *cursor++,
        *cursor++,
      });

      for (; cursor < polygon.end(); ++cursor) {
        line.add_point(*cursor);

        if (utils::algebra::distance::point_line(*cursor, line) >= threshold) {
          line.sub_point(*cursor--);
          break;
        }
      }

      auto e = cursor < polygon.end() ? *cursor : *polygon.begin();

      utils::algebra::segment segment {
        close_point({line.y(s.x), line.x(s.y)}, s),
        close_point({line.y(e.x), line.x(e.y)}, e)
      };

      lines.emplace_back(line, segment);
    }

    _p(intercepts, std::vector<utils::algebra::segment>);
    //
    for (auto pair = lines.begin(); pair < lines.end(); ++pair) {
      auto last_pair = pair - 1 >= lines.begin() ? pair - 1 : lines.end() - 1;
      auto next_pair = pair + 1 <  lines.end()   ? pair + 1 : lines.begin();

      auto s = utils::algebra::intercept::line_line(last_pair->first, pair->first);
      auto e = utils::algebra::intercept::line_line(next_pair->first, pair->first);

      intercepts.push_back({
        utils::algebra::distance::point_point(s, pair->second.s) < threshold ? s : pair->second.s,
        utils::algebra::distance::point_point(e, pair->second.e) < threshold ? e : pair->second.e
      });
    }

    _p(result, utils::polygon::polygon);
    //
    for (auto segment = intercepts.begin(); segment < intercepts.end(); ++segment) {
      auto next_segment = segment + 1 < intercepts.end() ? segment + 1 : intercepts.begin();

      result.push_back((segment->e + next_segment->s) / 2);
    }

    return result;
  }

core::utils::polygon::polygon
  core::utils::polygon::simplify_inline(
    _p(polygon  , core::utils::polygon::polygon const&),
    _p(threshold, double)
  ) {
    using namespace core;

    _p(lines, std::vector<utils::algebra::segment>);
    //
    for (auto cursor = polygon.begin(); cursor < polygon.end();) {
      auto segment = core::utils::algebra::segment{
        *cursor++,
        *cursor++,
      };

      for (; cursor < polygon.end(); ++cursor) {
        if (utils::algebra::distance::point_segment(*cursor, segment) >= threshold) {
          --cursor;
          break;
        }

        segment.e = *cursor;
      }

      lines.push_back(segment);
    }

    _p(result, utils::polygon::polygon);
    //
    for (auto segment = lines.begin(); segment < lines.end(); ++segment) {
      result.push_back(segment->e);
    }

    return result;
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
        auto length = utils::algebra::distance::point_point(*past_point, *last_point);
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
            length += utils::algebra::distance::point_point(*last_point, *point);
            points.add_point(*point);

            if (utils::algebra::distance::point_line(*point, points) > threshold_curve) {
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