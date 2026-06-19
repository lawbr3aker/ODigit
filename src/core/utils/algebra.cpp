#include "algebra.hpp"

core::utils::algebra::point
  core::utils::algebra::line_eq::y(
    _p(x, double)
  ) const {
    return {x, x * slope + offset};
  }

core::utils::algebra::point
  core::utils::algebra::line_eq::x(
    _p(y, double)
  ) const {
    return {(y - offset) / slope, y};
  }

core::utils::algebra::linear_regression::linear_regression(
  _p(points, std::vector<core::utils::algebra::point> const&)
) : line_eq() {
  for (auto const& point: points)
    add_point(point);
}

void
  core::utils::algebra::linear_regression::add_point(
    _p(point, core::utils::algebra::point const&)
  ) {
    ++n;

    sum_x  += point.x;
    sum_y  += point.y;
    sum_xy += point.x * point.y;
    sum_xx += point.x * point.x;

    calculate();
  }

void
  core::utils::algebra::linear_regression::sub_point(
    _p(point, core::utils::algebra::point const&)
  ) {
    --n;

    sum_x  -= point.x;
    sum_y  -= point.y;
    sum_xy -= point.x * point.y;
    sum_xx -= point.x * point.x;

    calculate();
  }

void
  core::utils::algebra::linear_regression::calculate(
  ) {
    if (n < 2) {
        slope  = 0;
        offset = (n == 1 ? sum_y : 0);
        return;
    }
    slope  = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
    offset = (sum_y - slope * sum_x) / n;
  }

core::utils::algebra::linear_regression &
  core::utils::algebra::linear_regression::merge(
    _p(other, core::utils::algebra::linear_regression const&)
  ) {
    n += other.n;
    sum_x += other.sum_x;
    sum_y += other.sum_y;
    sum_xy += other.sum_xy;
    sum_xx += other.sum_xx;

    calculate();

    return *this;
  }

double
  core::utils::algebra::distance::point_line(
    _p(point, core::utils::algebra::point   const&),
    _p(line , core::utils::algebra::line_eq const&)
  ) {
    return std::fabs(line.slope * point.x - point.y + line.offset) / std::sqrt(line.slope * line.slope + 1);
  }

double
  core::utils::algebra::distance::point_segment(
    _p(point  , core::utils::algebra::point const&),
    _p(segment, core::utils::algebra::segment const&)
  ) {
    using namespace core;

    utils::algebra::point ps{point.x - segment.s.x, point.y - segment.s.y};
    utils::algebra::point es{segment.e.x - segment.s.x, segment.e.y - segment.s.y};

    return std::abs((ps.x * es.y - ps.y * es.x) / std::sqrt(es.x * es.x + es.y * es.y));
  }

double
  core::utils::algebra::distance::point_point(
    _p(a, core::utils::algebra::point const&),
    _p(b, core::utils::algebra::point const&)
  ) {
    return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
  }


core::utils::algebra::point
  core::utils::algebra::intercept::line_line(
    _p(a, core::utils::algebra::line_eq const&),
    _p(b, core::utils::algebra::line_eq const&)
  ) {
    auto x = (b.offset - a.offset) / (a.slope - b.slope);
    auto y = a.slope * x + a.offset;

    return {x, y};
  }

double
  core::utils::algebra::angle::line_line(
    _p(a, core::utils::algebra::line_eq const&),
    _p(b, core::utils::algebra::line_eq const&)
  ) {
    return std::atan(std::abs((b.slope - a.slope) / (1 + a.slope * b.slope)));
  }

double
  core::utils::algebra::angle::vertices(
    _p(a, core::utils::algebra::point const&),
    _p(b, core::utils::algebra::point const&),
    _p(c, core::utils::algebra::point const&)
  ) {
    utils::algebra::point ba{a.x - b.x, a.y - b.y};
    utils::algebra::point bc{c.x - b.x, c.y - b.y};

    return std::acos((ba.x * bc.x + ba.y * bc.y) / (std::sqrt(ba.x * ba.x + ba.y * ba.y) * (std::sqrt(bc.x * bc.x + bc.y * bc.y))));
  }