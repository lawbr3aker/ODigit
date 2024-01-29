#pragma once

#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>

#include "../macros.h"

namespace core::utils::algebra {
  typedef struct {
    _p(x, double);
    _p(y, double);

    operator cv::Point() const { return {int(x), int(y)}; }
  } point;

  typedef struct {
    _p(s, point);
    _p(e, point);
  } line;

  typedef struct {
    _p(slope , double) = 0;
    _p(offset, double) = 0;

    [[nodiscard]]
    utils::algebra::point
      y(
        _p(x, double)
      ) const;
  } line_eq;

  class linear_regression : public utils::algebra::line_eq {
   public:
    explicit
    linear_regression(
      _p(points, std::vector<utils::algebra::point> const&) = {}
    );

    void
      add_point(
        _p(point, utils::algebra::point const&)
      );

    void
      calculate(
      );

    linear_regression &
      merge(
        _p(other, utils::algebra::linear_regression const&)
      );

   public:
    _p(n     , int) = 0;
    _p(sum_x , double) = 0;
    _p(sum_y , double) = 0;
    _p(sum_xy, double) = 0;
    _p(sum_xx, double) = 0;
  };

  double
    point2line(
      _p(point, utils::algebra::point const&),
      _p(line , utils::algebra::line  const&)
    );

  double
    point2line(
      _p(point  , utils::algebra::point   const&),
      _p(line_eq, utils::algebra::line_eq const&)
    );

  double
    point2point(
      _p(point_s, utils::algebra::point const&),
      _p(point_e, utils::algebra::point const&)
    );

  namespace intercept {
    utils::algebra::point
      line_line(
        _p(a, utils::algebra::line_eq const&),
        _p(b, utils::algebra::line_eq const&)
      );
  }
  namespace angle {
    double
      line_line(
        _p(a, utils::algebra::line_eq const&),
        _p(b, utils::algebra::line_eq const&)
      );
    double
      vertices(
        _p(a, utils::algebra::point const&),
        _p(b, utils::algebra::point const&),
        _p(c, utils::algebra::point const&)
      );
  }
}