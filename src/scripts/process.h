#pragma once

#include "editor.h"

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QFileDialog>
#include <QTextCodec>
#include <QApplication>
#include <QProcess>

#include <utility>
#include <numeric>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

#include <vector>
#include <algorithm>
#include <cmath>

#include <dxflib/dl_dxf.h>

#include "config.h"
#include <windows.h>

extern int main(int argc, char *argv[]);

class Contour {
public:
    explicit Contour(std::vector<cv::Point> points)
            : _points(std::move(points)) {
    }

    [[nodiscard]] std::vector<cv::Point> const &points() const {
        return _points;
    }

    [[nodiscard]] double area() const {
        auto area = cv::contourArea(_points);

        return area;
    }

    cv::Point center() const {
        auto moments = cv::moments(_points);
        auto center = cv::Point(moments.m10 / moments.m00, moments.m01 / moments.m00);

        return center;
    }

private:
    std::vector<cv::Point> _points;
};

class Process : public QObject {
   Q_OBJECT
   Q_PROPERTY(Editor* editor READ editor WRITE setEditor)
   Q_PROPERTY(bool loaded MEMBER loaded NOTIFY loadedChanged)
   Q_PROPERTY(bool processed MEMBER processed)
   Q_PROPERTY(Config * config MEMBER config)
signals:
    void loadedChanged();
public:
    bool loaded = false;
    bool processed = false;

    Config *config = Config::global;

    explicit Process()
    : QObject(nullptr) {
    }

    void setEditor(Editor * editor) {
       _editor = editor;
    }

    [[nodiscard]] Editor * editor() const {
        return _editor;
    }

    Q_INVOKABLE void restartProgram() {
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }

    Q_INVOKABLE void readPath() {
        auto const& path = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open image"), nullptr, QObject::tr("Image File (*.png, *.jpg)"));
        if (path.length() == 0)
            return;

        openFile(path);
    }

    Q_INVOKABLE void openFile(QString const& path) {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        cv::Mat dst;
        QByteArray data = file.readAll();
        cv::Mat input = cv::imdecode(cv::Mat(data.size(), 1, CV_8U, data.data()), cv::IMREAD_COLOR);

        cv::aruco::DetectorParameters detector_parameters = cv::aruco::DetectorParameters();
        cv::aruco::Dictionary         detector_dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
        //
        cv::aruco::ArucoDetector detector(detector_dictionary, detector_parameters);
        cv::Size2f const
            plane_size(Config::global->value("plane_width").value<float>(),
                       Config::global->value("plane_height").value<float>());
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

            if (!(corner_tl && corner_tr && corner_br && corner_bl)) {
                bool last = false;

                QString corners_error;
                if (!corner_tl) {
                  corners_error += "Top left"; last = true;
                }
                if (!corner_tr) {
                  if (last) corners_error += " & ";
                  corners_error += "Top right"; last = true;
                }
                if (!corner_bl) {
                  if (last) corners_error += " & ";
                  corners_error += "Bottom left"; last = true;
                }
                if (!corner_br) {
                  if (last) corners_error += " & ";
                  corners_error += "Bottom right"; last = true;
                }
                qDebug() << "Corners " << corners_error << " not found!";

                return;
            }
        }

        loaded = true;

        auto const pixels = Config::global->value("cm_pixels").value<float>();
        cv::Size2f const
            plane_size_warped(
                plane_size.width * pixels,
                plane_size.height * pixels
            );

        std::vector<cv::Point2f>
            plane_corners_warped{
                {0                        , 0},
                {plane_size.width * pixels, 0},
                {plane_size.width * pixels, plane_size.height * pixels},
                {0                        , plane_size.height * pixels}
            };

        auto warp_matrix = cv::getPerspectiveTransform(plane_corners, plane_corners_warped);

        _warped = new cv::Mat();
        cv::warpPerspective(input, *_warped, warp_matrix, plane_size_warped);

        _image = new cv::Mat(*_warped);

        _editor->setImage(*_image);
        _editor->setZoom(defaultZoom());

        cv::cvtColor(*_image, *_image, cv::COLOR_RGB2GRAY);
        cv::threshold(*_image, *_image, 100, 255, cv::THRESH_BINARY);
    }

    Q_INVOKABLE void process(float compress=1) {
        auto image = _image->clone();
        if (compress != 1) {
            auto size = cv::Size2f (image.cols, image.rows);
            size.width *= compress;
            size.height *= compress;
            cv::resize(image, image, size);
        }

        qDeleteAll(_editor->points.begin(), _editor->points.end());
        _editor->points.clear();
        qDeleteAll(_editor->lines.begin(), _editor->lines.end());
        _editor->lines.clear();
        _contours.clear();

        cv::GaussianBlur(image, image,
                         cv::Size(
                                 Config::global->value("scanner/blur_size").value<int>(),
                                 Config::global->value("scanner/blur_size").value<int>()),
                         -1);
        //
        cv::Mat edges;
        cv::Canny(image, edges,
                  Config::global->value("scanner/threshold_1").value<int>(),
                  Config::global->value("scanner/threshold_2").value<int>());
        auto size = Config::global->value("scanner/fixer/erode_dilate_size").value<int>();
        auto kernel = cv::getStructuringElement(
                cv::MORPH_ELLIPSE,
                cv::Size(2 * size + 1,2 * size + 1),
                cv::Point(size, size));
        //
        cv::dilate(edges, edges, kernel);
        cv::erode(edges, edges, kernel);

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(edges, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        std::vector<Contour> cs;
        auto min_area = Config::global->value("scanner/fixer/min_area").value<int>();
        for (auto const& points : contours) {
            auto const& contour = Contour(points);

            if (contour.area() < min_area)
                continue;

            cs.push_back(contour);
        }

        for (int contourI = 0; contourI < cs.size(); ++contourI) {
            std::vector<int> deleteContours;
            bool deleteSelf = false;

            auto const& contour = cs[contourI];
            for (int otherI = contourI + 1; otherI < cs.size(); ++otherI) {
                auto const& other = cs[otherI];
                //
                if (cv::norm(contour.center() - other.center()) > 7)
                    continue;

                if (contour.points().size() < other.points().size())
                    deleteContours.push_back(otherI);
                else {
                    deleteSelf = true;
                    break;
                }
            }

            if (deleteSelf) {
                cs.erase(cs.begin() + contourI);
                --contourI;
            } else {
                for (auto const& i : deleteContours)
                    cs.erase(cs.begin() + i);
            }
        }

        std::vector<std::vector<cv::Point>> fixed_contours(cs.size());
        int off = 0;
        for (int i = 0; i < cs.size(); ++i) {
            auto const& contour = cs[i];
            auto      & fixed   = fixed_contours[i];

            std::vector<cv::Point> to_fix = contour.points();
            auto to_fix_mat = cv::Mat(_warped->size(), CV_8UC1, {0, 0, 0});

            std::vector<cv::Vec4i> lines;
            HoughLinesP(to_fix_mat, lines, 1, CV_PI / 180,
                        Config::global->value("scanner/fixer/threshold").value<int>(),
                        Config::global->value("scanner/fixer/min_line_length").value<double>(),
                        Config::global->value("scanner/fixer/max_line_gap").value<double>());

            cv::approxPolyDP(to_fix, fixed,
                             Config::global->value("scanner/fixer/epsilon").value<double>(),
                             true);

            _contours.emplace_back();
            float inv_scale = 1 / compress;
            auto & cnt = _contours.back();
            for (auto const& point : fixed) {
                auto p = new Point(point.x * inv_scale, point.y * inv_scale);

                cnt.push_back(p);
                _editor->points.push_back(p);
            }

            for (int i = 0, j = 1; i < fixed.size(); ++i, j++) {
                if (i == fixed.size() - 1) {
                    j = 0;
                }

                _editor->lines.push_back(new Line(_editor->points[off + i], _editor->points[off + j]));
            }
            off += fixed.size();
        }

        processed = true;
    }

    Q_INVOKABLE [[nodiscard]] float defaultZoom() const {
        if (_image) {
            if (_editor->width() > _editor->height())
                return _editor->height() / _image->rows;
            else
                return _editor->width() / _image->cols;
        }
        return 1;
    }

    Q_INVOKABLE void exportDXF() {
        auto const& path = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save a file"), "output.dxf", QObject::tr("DXF File (*.dxf)"));
        if (path.length() == 0)
            return;

        DL_Dxf dxf;
        DL_WriterA* dw = dxf.out(path.toStdString().c_str(), DL_Codes::AC1015);
        if (dw == nullptr) {
        }

        dxf.writeHeader(*dw);
        dw->dxfString(9, "$INSUNITS");
        dw->dxfInt(70, 5);
        // real (double, float) variable:
        dw->dxfString(9, "$DIMEXE");
        dw->dxfReal(40, 1.25);
        // string variable:
        dw->dxfString(9, "$TEXTSTYLE");
        dw->dxfString(7, "Standard");
        // vector variable:
        dw->dxfString(9, "$LIMMIN");
        dw->dxfReal(10, 0.0);
        dw->dxfReal(20, 0.0);
        dw->sectionEnd();

        dw->sectionTables();
        dxf.writeVPort(*dw);
        dw->tableEnd();

        dw->tableLayers(0);
        dxf.writeLayer(
            *dw,
            DL_LayerData("0", 0),
            DL_Attributes("", DL_Codes::black, 100, "CONTINUOUS"));
        dw->tableEnd();

        dxf.writeStyle(*dw);
        dxf.writeView(*dw);
        dxf.writeUcs(*dw);
        dw->tableAppid(1);
        dw->tableAppidEntry(0x12);
        dw->dxfString(2, "ACAD");
        dw->dxfInt(70, 0);
        dw->tableEnd();

        dxf.writeBlockRecord(*dw);
        dw->tableEnd();
        dw->sectionEnd();

        dw->sectionBlocks();
        dw->sectionEnd();

        dw->sectionEntities();

        auto const pixels = Config::global->value("cm_pixels").value<float>();

        for (auto const& contour : _contours) {
            for (int i = 0, j = 1; i < contour.size(); ++i, j++) {
                if (i == contour.size() - 1) {
                    j = 0;
                }

                auto const& point = contour[i], next = contour[j];

                dxf.writeLine(
                    *dw,
                    DL_LineData(
                            point->x / pixels, (_image->rows - point->y) / pixels, 0,
                            next->x / pixels, (_image->rows - next->y) / pixels, 0),
                    DL_Attributes("", 256, -1, "BYLAYER"));
            }
        }

        for (auto const& text: _editor->texts) {
            dxf.writeText(
                    *dw,
                    DL_TextData(
                            text->x / text->scale / pixels, (_image->rows - text->y / text->scale) / pixels, 0,
                            text->x / text->scale / pixels, (_image->rows - text->y / text->scale) / text->scale / pixels, 0,
                            text->size / text->scale / pixels, 1,
                            0,0,0,
                            text->text.toStdString().c_str(),
                            std::to_string(text->size / text->scale / 1.3) + "px arial", 0),
                            DL_Attributes("", 256, -1, "BYLAYER")
                    );
        }

        dw->sectionEnd();

        dxf.writeObjects(*dw);
        dxf.writeObjectsEnd(*dw);
        dw->dxfEOF();
        dw->close();
        delete dw;
    }
private:
    std::vector<std::vector<Point *>> _contours;
    cv::Mat *_image = nullptr;
    cv::Mat *_warped = nullptr;
    Editor *_editor = nullptr;
    double ratio;
};
