#pragma once

#include <QDebug>

#include <vector>
#include <cmath>
#include <random>

#include "./algebra.hpp"

#include "../macros.h"

namespace core::utils::polygon {
  typedef struct {
    _p(width , double);
    _p(height, double);
  } rect;

  typedef std::vector<utils::algebra::point> polygon;
  typedef std::vector<utils::polygon::polygon> polygon_list;

  utils::polygon::polygon
    simplify_midline(
      _p(polygon, utils::polygon::polygon const&),
      //
      _p(threshold, double)
    );

  utils::polygon::polygon
    simplify_slope(
      _p(polygon, utils::polygon::polygon const&),
      //
      _p(threshold_angle , double),
      _p(threshold_length, double),
      _p(threshold_curve , double)
    );

  utils::polygon::polygon
    simplify_rdp(
      _p(polygon, utils::polygon::polygon const&),
      //
      _p(threshold_height, float),
      _p(threshold_width , float)
    );
}