#pragma once

#include "editor.h"

#include <QObject>
#include <QDebug>
#include <QFileDialog>

#include <utility>
#include <numeric>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/opencv.hpp>

#include <ranges>
#include <vector>
#include <algorithm>
#include <cmath>

class Process : public QObject {
   Q_OBJECT
   Q_PROPERTY(Editor* editor READ editor WRITE setEditor)
public:
    explicit Process()
    : QObject(nullptr) {
    }

    void setEditor(Editor * editor) {
       _editor = editor;
    }

    [[nodiscard]] Editor * editor() const {
        return _editor;
    }

    Q_INVOKABLE void readPath() {
        QString const& path = "/home/lawbr3aker/Works/C++/UntitledX2JF8/data/camera9.jpg";

        openFile(path);
    }

    Q_INVOKABLE void openFile(QString const& path) {
        cv::Mat input = cv::imread(path.toStdString(), cv::IMREAD_COLOR);

        auto static detector_params = cv::aruco::DetectorParameters();
        auto static detector =
            cv::aruco::ArucoDetector(
                cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50),
                detector_params
            );

        cv::Size2f const
            plane_size(144, 94);
        std::vector<cv::Point2f>
            plane_corners(4);
        //
        {
            std::vector<std::vector<cv::Point2f>> coordinates;
            std::vector<int> ids;
            //
            detector.detectMarkers(input, coordinates, ids);
            // find corners
            bool
                corner_tl = false,
                corner_tr = false,
                corner_br = false,
                corner_bl = false;

            int
                id;
            std::vector<cv::Point2f>
                corners;
            for (int i = 0; i < ids.size(); ++i) {
                id      = ids[i];
                corners = coordinates[i];

                switch (id) {
                    case 11: plane_corners[0] = corners[2], corner_tl = true; break;
                    case 12: plane_corners[1] = corners[3], corner_tr = true; break;
                    case 13: plane_corners[2] = corners[0], corner_br = true; break;
                    case 14: plane_corners[3] = corners[1], corner_bl = true; break;
                    default:;
                }
            }

            if (not (corner_tl && corner_tr && corner_br && corner_bl)) {
                qDebug() << "Not all corners found!\n";
            }
        }

        qDebug() << "Succeed\n";

        cv::Size2f const
            plane_size_warped(
                float(input.cols),
                float(input.cols / plane_size.aspectRatio())
            );
        std::vector<cv::Point2f>
            plane_corners_warped{
                {0                      , 0},
                {plane_size_warped.width, 0},
                {plane_size_warped.width, plane_size_warped.height},
                {0                      , plane_size_warped.height}
            };

        auto warp_matrix = cv::getPerspectiveTransform(plane_corners, plane_corners_warped);

        cv::Mat warped;
        cv::warpPerspective(input, warped, warp_matrix, plane_size_warped);

        auto & image = warped;
        cvtColor(image, image, cv::COLOR_BGR2RGB);
        cv::imshow("asdfasdf", image);

        cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
        cv::threshold(image, image, 170, 255, cv::THRESH_BINARY);

        cv::Mat dest;
        cvtColor(image, dest, cv::COLOR_BGR2RGB);

        float s = plane_size_warped.width / plane_size.width / 56;
        cv::resize(dest, dest, cv::Size2f(s * plane_size_warped.width, s * plane_size_warped.height));

        _editor->setImage(dest);
    }

private:
    Editor *_editor = nullptr;
};
