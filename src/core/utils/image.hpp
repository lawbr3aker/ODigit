#pragma once

#include <utility>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>

#include <QDebug>
#include <QFile>

#include <opencv2/opencv.hpp>

#include "../macros.h"

namespace core::utils::image {
  typedef cv::Mat image;

  class contour: public std::vector<cv::Point> {
   public:
    contour(
    ): std::vector<cv::Point>() {}

    explicit
    contour(
      _p(other, std::vector<cv::Point>)
    ): std::vector<cv::Point>(std::move(other)) {}

    [[nodiscard]]
    cv::Point2d
      center() const;

    [[nodiscard]]
    double
      area() const;
  };

  typedef std::vector<utils::image::contour> contour_list;

  utils::image::image
    open(
      _p(path, std::string const&)
    );
}
