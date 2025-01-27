#include "./process.hpp"

#define RES 4000.f

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

    for (auto & polyline : _editor->_polylines) {
      qDeleteAll(polyline->segments);
      polyline->segments.clear();
      _editor->_polylines.removeOne(polyline);
    }
    qDeleteAll(_editor->_points);
    _editor->_points.clear();

    _step = 1;
  }

void
  core::gui::scripts::process::step_detect(
  ) {
    if (_step != 1)
      return;

    if (_config->value("plane_width").value<float>() == 0 or _config->value("plane_height").value<float>() == 0)
      return;

    cv::Mat temp;
    cv::cvtColor(*_image, temp, cv::COLOR_BGR2GRAY);

    cv::aruco::ArucoDetector
      detector(
        cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50),
        cv::aruco::DetectorParameters()
      );

    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> coordinates;
    //
    detector.detectMarkers(temp, coordinates, ids);

    // find corners
    cv::Point2f
      * corner_tl = nullptr,
      * corner_tr = nullptr,
      * corner_br = nullptr,
      * corner_bl = nullptr;

    for (int i = 0; i < ids.size(); ++i) {
      switch (ids[i]) {
        case 11: corner_tl = &coordinates[i][2]; break;
        case 12: corner_tr = &coordinates[i][3]; break;
        case 13: corner_br = &coordinates[i][0]; break;
        case 14: corner_bl = &coordinates[i][1]; break;
        default:;
      }
    }

    if (!(corner_tl && corner_tr && corner_br && corner_bl)) {
      _step = 0;
      return;
    }

    auto pixels = _config->value("cm_pixels").value<float>();
    //
    QSizeF
      size {
        (float) _image->cols,
        (float) _image->rows
      },
      size_warped = size * (RES / std::max(size.width(), size.height()));

    _size.width = (float) size_warped.width();
    _size.height = (float) size_warped.height();

    std::vector<cv::Point2f>
      corners {
        *corner_tl,
        *corner_tr,
        *corner_br,
        *corner_bl,
      },
      corners_warped{
        {0          , 0},
        {_size.width, 0},
        {_size.width, _size.height},
        {0          , _size.height},
      };

    auto warp_matrix = cv::getPerspectiveTransform(corners, corners_warped);

    cv::warpPerspective(*_image, *_image, warp_matrix, _size);

    _editor->set_image(
      &*_image,
      _config->value("plane_width").value<float>() * pixels,
      _config->value("plane_height").value<float>() * pixels
      );
    _editor->home();

    _step = 2;
  }

std::tuple<int, int> _(cv::Mat const& src) {
  int channels[1] = {0};
  int histSize[1] = {256};
  float histRanges[2] = {0.0, 256.0};
  const float *ranges[1] = {histRanges};

  cv::MatND hist;
  calcHist(&src, 1, channels, cv::Mat(), hist, 1, histSize, ranges);

  auto *histogram = (unsigned char *) (hist.data);

  double optimalThresh1, optimalThresh2;
  double W0K(0), W1K, W2K, W3K, M0, M1, M2, M3, currVarB, maxBetweenVar(0), M0K(0), M1K, M2K, M3K, MT(0);

  int N = src.rows*src.cols;

  for (int k = 0; k <= 255; k++) {
    MT += k * (histogram[k] / (double) N);
  }

  for (int t1 = 0; t1 <= 255; t1++) {
    W0K += histogram[t1] / (double) N;
    M0K += t1 * (histogram[t1] / (double) N);
    M0 = M0K / W0K;

    W1K = 0;
    M1K = 0;

    for (int t2 = t1 + 1; t2 <= 255; t2++) {
      W1K += histogram[t2] / (double) N;
      M1K += t2 * (histogram[t2] / (double) N);
      M1 = M1K / W1K;
      W2K = 1 - (W0K + W1K);
      M2K = MT - (M0K + M1K);

      if (W2K <= 0)
        break;

      W2K = 0;
      M2K = 0;

      for (int t3 = t2 + 1; t3 <= 255; t3++) {
        W2K += histogram[t3] / (double) N;
        M2K += t3 * (histogram[t3] / (double) N);
        M2 = M2K / W2K;
        W3K = 1 - (W0K + W1K + W2K);
        M3K = MT - (M0K + M1K + M2K);

        M3 = M3K / W3K;
        currVarB = W0K * (M0 - MT) * (M0 - MT) + W1K * (M1 - MT) * (M1 - MT) + W2K * (M2 - MT) * (M2 - MT) + W3K * (M3 - MT) * (M3 - MT);

        if (maxBetweenVar < currVarB) {
          maxBetweenVar = currVarB;
          optimalThresh1 = t1;
          optimalThresh2 = t2;
        }
      }
    }
  }

  return std::make_tuple(optimalThresh1, optimalThresh2);
}

void
  core::gui::scripts::process::step_process(
  ) {
    if (_step != 2 and _step != 3)
      return;

    auto pixels  = _config->value("cm_pixels").value<float>();
    auto scale_w = _config->value("plane_width").value<float>() / (_size.width / pixels);
    auto scale_h = _config->value("plane_height").value<float>() / (_size.height / pixels);

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
    
    auto ths = _(image);
    
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

    cv::Mat edges;
    cv::Canny(
      image, edges,
      std::get<0>(ths), std::get<1>(ths)
    );

    auto size = _config->value("scanner/fixer/dilate_size").value<int>();
    auto kernel = cv
      ::getStructuringElement(
        cv::MORPH_ELLIPSE,
        cv::Size(2 * size + 1,2 * size + 1),
        cv::Point(size, size)
      );
    cv::dilate(edges, edges, kernel);
    cv::erode(edges, edges, kernel);

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
      if (contour.area() / pixels / pixels < filter_min_area)
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
        for (auto const &point: fixed) {
          contour_final.push_back({
            (float) point.x * scale_w,
            (float) point.y * scale_h,
          });
        }
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