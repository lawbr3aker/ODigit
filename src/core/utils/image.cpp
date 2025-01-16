#include "./image.hpp"

cv::Point2d
  core::utils::image::contour::center(
  ) const {
    auto moments = cv::moments(*this);

    return cv
      ::Point2d {
        moments.m10 / moments.m00,
        moments.m01 / moments.m00
      };
  }

double
  core::utils::image::contour::area(
  ) const {
    return cv::contourArea(*this);
  }

core::utils::image::image
  core::utils::image::open(
    _p(path, std::string const&)
  ) {
    QFile file(path.c_str()); file.open(QFile::ReadOnly);

    return cv
      ::imdecode(
        cv::Mat(file.size(), 1, CV_8U, file.readAll().data()),
        cv::IMREAD_COLOR
      );
  }
