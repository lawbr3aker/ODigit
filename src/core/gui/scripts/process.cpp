#include "./process.hpp"
void shimage(cv::Mat const& image, int width = 1200);
core::gui::components::editor *
  core::gui::scripts::process::set_editor(
    _p(value, core::gui::components::editor *)
  ) {
    QQmlEngine::setObjectOwnership(value, QQmlEngine::CppOwnership);

    if (value)
      return _editor = value;
    return _editor;
  }

core::gui::scripts::config *
  core::gui::scripts::process::set_config(
      _p(value, core::gui::scripts::config *)
  ) {
    if (value)
      return _config = value;
    return _config;
  }

void
  core::gui::scripts::process::step_path(
    _p(path, QString const&)
  ) {
    QString path_final = path;
    if (path_final == "")
      path_final = QFileDialog
        ::getOpenFileName(
          nullptr,
          "Open image",
          QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
          "Image File (*.png, *.jpg)"
        );

    if (path_final == "")
      return;

    _image = std::make_unique<core::utils::image::image>(core::utils::image::open(path_final.toStdString()));

    if (not _contours.empty())
      _contours.clear();

    _step = 1;
  }

void
  core::gui::scripts::process::step_detect(
  ) {
    if (_step != 1)
      return;

    cv::aruco::ArucoDetector
      detector(
        cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50),
        cv::aruco::DetectorParameters()
      );

    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> coordinates;
    //
    detector.detectMarkers(*_image, coordinates, ids);

    // find corners
    cv::Point2f
      * corner_tl = nullptr,
      * corner_tr = nullptr,
      * corner_br = nullptr,
      * corner_bl = nullptr;

    for (int i = 0; i < ids.size(); ++i) {
      switch (ids[i]) {
        case 11: corner_br = &coordinates[i][2]; break;
        case 12: corner_bl = &coordinates[i][3]; break;
        case 13: corner_tl = &coordinates[i][0]; break;
        case 14: corner_tr = &coordinates[i][1]; break;
        default:;
      }
    }

    if (!(corner_tl && corner_tr && corner_br && corner_bl)) {
      _step = 0;
      return;
    }

    auto pixels = _config->value("cm_pixels").value<float>();
    //
    cv::Size2f
      size {
        _config->value("plane_width").value<float>(),
        _config->value("plane_height").value<float>()
      },
      size_warped {
        size.width  * pixels,
        size.height * pixels,
      };

    std::vector<cv::Point2f>
      corners {
        *corner_tl,
        *corner_tr,
        *corner_br,
        *corner_bl,
      },
      corners_warped{
        {0                , 0},
        {size_warped.width, 0},
        {size_warped.width, size_warped.height},
        {0                , size_warped.height},
      };

    auto warp_matrix = cv::getPerspectiveTransform(corners, corners_warped);

    cv::warpPerspective(*_image, *_image, warp_matrix, size_warped);

    _step = 2;

    cv::imwrite("output.jpg", *_image);

    _editor->set_image(&*_image);
    _editor->home();
  }

void
  core::gui::scripts::process::step_process(
  ) {
    if (_step != 2 and _step != 3)
      return;

    core::utils::image::image image = _image->clone();

    cv::GaussianBlur(
      image, image,
      cv::Size2i {
        _config->value("scanner/blur_size").value<int>(),
        _config->value("scanner/blur_size").value<int>()
      },
      -1
    );

    cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
    cv::threshold(image, image, 100, 255, cv::THRESH_BINARY);

    //
    cv::Mat edges;
    cv::Canny(
      image, edges,
      _config->value("scanner/threshold_1").value<int>(),
      _config->value("scanner/threshold_2").value<int>()
    );
    auto size = _config->value("scanner/fixer/ed_size").value<int>();
    auto kernel = cv
      ::getStructuringElement(
        cv::MORPH_ELLIPSE,
        cv::Size(2 * size + 1,2 * size + 1),
        cv::Point(size, size)
      );
    cv::dilate(edges, edges, kernel);

    core::utils::image::contour_list contours;
    std::vector<std::vector<cv::Point>> contours_temp;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(
      edges, contours_temp,
      hierarchy,
      cv::RETR_TREE,
      cv::CHAIN_APPROX_SIMPLE
    );

    for (auto const& contour : contours_temp)
      contours.emplace_back(contour);

    _contours.clear();
    auto filter_min_area = _config->value("scanner/fixer/min_area").value<int>();
    auto filter_max_gap  = _config->value("scanner/fixer/max_gap").value<int>();
    auto filter_epsilon  = _config->value("scanner/fixer/epsilon").value<double>();
    //
    for (int contour_index = 0; contour_index < contours.size(); ++contour_index) {
      auto const& contour = contours[contour_index];
      if (contour.area() < filter_min_area)
        continue;

      //
      bool keep = true;
      for (int other_index = 0; other_index < contours.size(); ++other_index) {
        if (other_index == contour_index)
          continue;
        //
        auto const &other = contours[other_index];
        if (other.area() < filter_min_area)
          continue;
        //
        if (cv::norm(contour.center() - other.center()) > filter_max_gap)
          continue;

        if (contour.size() > other.size()) {
          keep = false;
          break;
        }
      }

      if (keep) {
        utils::image::contour fixed;
        cv::approxPolyDP(
          contour, fixed,
          filter_epsilon,
          true
        );
        //
        auto & contour_final = _contours.emplace_back();
        for (auto const &point: fixed)
          contour_final.push_back({
            (float) point.x,
            (float) point.y
          });
      }
    }

    _step = 3;

    for (auto & polyline : _editor->_polylines) {
      qDeleteAll(polyline->segments);
      polyline->segments.clear();
      _editor->_polylines.removeOne(polyline);
    }
    qDeleteAll(_editor->_points);
    _editor->_points.clear();

    for (auto & contour : _contours)
      _editor->add_polyline(contour, true);
  }

void
  core::gui::scripts::process::step_export_dxf(
    _p(path, QString const&)
  ) const {
    if (_step != 3)
      return;

    QString path_final = path;
    if (path_final == "")
      path_final = QFileDialog
        ::getSaveFileName(
          nullptr,
          "Output file",
          QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/output.dxf",
          "DXF format (*.dxf)"
        );

    if (path_final == "")
      return;

    auto ratio = 1. / _config->value("cm_pixels").value<double>();

    _editor->export_dxf(path_final, ratio, -ratio);
  }