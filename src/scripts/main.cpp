#include <utility>
#include <numeric>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/opencv.hpp>

#include <ranges>
#include <vector>
#include <dxflib/dl_dxf.h>
#include <algorithm>
#include <spline/BSpline.h>
#include <curve/CurveCSS.h>
#include <cmath>

void draw(cv::Mat & dst, std::vector<cv::Point> const& points, cv::Scalar const& color={0, 0, 255}) {
    auto const points_count = points.size();

    for (auto [index, point] : std::views::enumerate(points)) {
        std::cout << point << '\n';
        auto index_next = index + 1 < points_count ? index + 1 : 0;
        //
        cv::line(dst, point, points[index_next], color, 2);
    }
}

class Contour {
    std::vector<cv::Point> _points;

  public:

    explicit Contour(std::vector<cv::Point> points)
      : _points(std::move(points)) {
    }

    auto const& points() const {
        return _points;
    }

    auto area() const {
        auto area = cv::contourArea(_points);

        return area;
    }

    cv::Point center() const {
        auto moments = cv::moments(_points);
        auto center = cv::Point(moments.m10 / moments.m00, moments.m01 / moments.m00);

        return center;
    }
};


void generateDXF(std::vector<std::vector<cv::Point>> const& contours, double scale) {
    DL_Dxf dxf;
    DL_WriterA* dw = dxf.out("myfile.dxf", DL_Codes::AC1015);
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

    for (auto const& contour : contours) {
        for (auto [point, next] : std::views::zip(contour, std::vector(contour.begin() + 1, contour.end()))) {
            dxf.writeLine(
                *dw,
                DL_LineData(point.x * scale, point.y * scale, 0, next.x * scale, next.y * scale, 0),
                DL_Attributes("", 256, -1, "BYLAYER"));
        }
    }

    dw->sectionEnd();

    dxf.writeObjects(*dw);
    dxf.writeObjectsEnd(*dw);
    dw->dxfEOF();
    dw->close();
    delete dw;
}

void show(cv::Mat const& image, std::string const& name="Main", int resize=1500) {
    static cv::Mat output;
    if (resize) {
        cv::resize(image, output, {resize, (int) (resize / image.size().aspectRatio())});
        cv::imshow(name, output);
    } else
        cv::imshow(name, image);
    cv::waitKey(0);
}

std::vector<cv::Point> denoise(std::vector<cv::Point> const& points, cv::Mat const& test);

int main() {
    const auto w = 1400;
    cv::Mat o;
    cv::Mat input = cv::imread("data/2process/با زاویه (2).JPG");
//    cv::resize(input, input, {1000, (int) (1000. / input.cols * input.rows)});

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
        for (auto [id, corners] : std::views::zip(ids, coordinates)) {
            switch (id) {
                case 11: plane_corners[0] = corners[2], corner_tl = true; break;
                case 12: plane_corners[1] = corners[3], corner_tr = true; break;
                case 13: plane_corners[2] = corners[0], corner_br = true; break;
                case 14: plane_corners[3] = corners[1], corner_bl = true; break;
                default:;
            }
        }

        if (not (corner_tl && corner_tr && corner_br && corner_bl)) {
            std::cout << "Not all corners found!";
            return -1;
        }
    }

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
    cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
    cv::threshold(image, image, 170, 255, cv::THRESH_BINARY);
//    show(image, "Image");
    cv::Mat p;
    cv::resize(image, p, {1000, (int) (1000 / image.size().aspectRatio())});
    cv::imwrite("img.png", p);
//    cv::imshow("Edges", image);
//    cv::waitKey(0);
    cv::GaussianBlur(image, image, cv::Size(3, 3), -1);
    //
    cv::Mat edges;
    cv::Canny(image, edges, 100, 100);
    auto s = 3;
    auto kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2*s + 1, 2*s + 1), cv::Point(s, s));
    cv::dilate(edges, edges, kernel);
    cv::erode(edges, edges, kernel);
//    show(edges, "Edges");

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(edges, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    std::vector<Contour> cs;
    for (auto const& points : contours) {
        auto const& contour = Contour(points);

        if (contour.area() < 300)
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
    for (auto const& [contour, fixed] : std::views::zip(cs, fixed_contours)) {
        std::vector<cv::Point> to_fix = contour.points();

        auto to_fix_mat = cv::Mat(warped.size(), CV_8UC1, {0, 0, 0});
        draw(to_fix_mat, to_fix, {255, 255, 255});

//        show(to_fix_mat, "To fix");

        auto drawing = to_fix_mat.clone();
        cv::cvtColor(drawing, drawing, cv::COLOR_GRAY2BGR);

        vector<Vec4i> lines;
        HoughLinesP(to_fix_mat, lines, 1, CV_PI / 180, 200, 100, 40);
        for (auto const& line : lines) {
            cv::line(drawing, {line[0], line[1]}, {line[2], line[3]}, {0, 0, 255}, 3);
        }
//        show(drawing, "drawing a");

//        double sigma = 1;
//        int M = round((10.0*sigma+1.0) / 2.0) * 2 - 1;
//        std:: cout<< M << "\n";
//        assert(M % 2 == 1); //M is an odd number
//        //create kernels
//        std::vector<double> g,dg,d2g; getGaussianDerivs(sigma,M,g,dg,d2g);
//        std::vector<double> curvex,curvey,smoothx,smoothy;
//        PolyLineSplit(fixed_points,curvex,curvey);
//        std::vector<double> X,XX,Y,YY;
//        getdXcurve(curvex,sigma,smoothx,X,XX,g,dg,d2g,false);
//        getdXcurve(curvey,sigma,smoothy,Y,YY,g,dg,d2g,false);
//        PolyLineMerge(fixed_points,smoothx,smoothy);

        cv::approxPolyDP(to_fix, fixed, 1.25, true);
        fixed.push_back(fixed[0]);
        denoise(fixed, image);
    }

    generateDXF(fixed_contours, plane_size.width / plane_size_warped.width);

    cv::Mat drawing = warped;
    cv::cvtColor(drawing, drawing, cv::COLOR_GRAY2RGB);
    for (auto const& contour : fixed_contours) {
        draw(drawing, contour);
    }
    cv::resize(drawing, o, {w, (int) (w / plane_size_warped.aspectRatio())});
    cv::imshow("Contours", o);
    cv::waitKey(0);
}

struct Arc {
    double      radius;
    cv::Point   center;
    cv::Point   start;
    cv::Point   end;
};

struct Line {
    cv::Point   start;
    cv::Point   end;
    double      slope;
};


double lineAngle(cv::Point const& line_s, cv::Point const& line_e) {
    return std::atan2(line_s.y - line_e.y, line_s.x - line_e.x) * 180 / M_PI;
}


double lineSlope(cv::Point const& line_s, cv::Point const& line_e) {
    return float(line_e.y - line_s.y) / float(line_e.x - line_s.x);
}

double lineLength(cv::Point const& line_s, cv::Point const& line_e) {
    return cv::norm(line_s - line_e);
}


double pointDistance2Line(cv::Point const& point, cv::Point const& line_s, cv::Point const& line_e) {
    double A = line_e.y - line_s.y;
    double B = line_s.x - line_e.x;
    double C = (line_e.x * line_s.y) - (line_s.x * line_e.y);

    return std::abs((A * point.x + B * point.y + C) / sqrt(A * A + B * B));
}

double pointDistance2Circle(cv::Point const& point, cv::Point const& circle_c, double circle_r) {
    return lineLength(point, circle_c) - circle_r;
}

void circleFit(std::vector<cv::Point> const& points, cv::Point2f &center, double &radius) {
    float xn = 0, xsum = 0;
    float yn = 0, ysum = 0;
    float n = points.size();

    for (int i = 0; i < n; i++)
    {
        xsum = xsum + points[i].x;
        ysum = ysum + points[i].y;
    }

    xn = xsum / n;
    yn = ysum / n;

    float ui = 0;
    float vi = 0;
    float suu = 0, suuu = 0;
    float svv = 0, svvv = 0;
    float suv = 0;
    float suvv = 0, svuu = 0;

    for (int i = 0; i < n; i++)
    {
        ui = points[i].x - xn;
        vi = points[i].y - yn;

        suu = suu + (ui * ui);
        suuu = suuu + (ui * ui * ui);

        svv = svv + (vi * vi);
        svvv = svvv + (vi * vi * vi);

        suv = suv + (ui * vi);

        suvv = suvv + (ui * vi * vi);
        svuu = svuu + (vi * ui * ui);
    }

    cv::Mat A = (cv::Mat_<float>(2, 2) <<
        suu, suv,
        suv, svv);

    cv::Mat B = (cv::Mat_<float>(2, 1) <<
        0.5*(suuu + suvv),
        0.5*(svvv + svuu));

    cv::Mat abc;
    cv::solve(A, B, abc);

    float u = abc.at<float>(0);
    float v = abc.at<float>(1);

    center.x = u + xn;
    center.y = v + yn;

    radius = sqrt(u * u + v * v + ((suu + svv) / n));
//    cv::Mat A(points.size(), 3, CV_32F);
//    cv::Mat B(points.size(), 1, CV_32F);
//
//    for(int index=0; index < points.size(); ++index) {
//        auto const& point = points[index];
//
//        A.at<float>(index, 0) = point.x;
//        A.at<float>(index, 1) = point.y;
//        A.at<float>(index, 2) = 1;
//        B.at<float>(index)       = point.x * point.x + point.y * point.y;
//    }
//    cv::Mat X;
//    cv::solve(A, B, X, cv::DECOMP_SVD);
//    center.x = X.at<float>(0) * 0.5f;
//    center.y = X.at<float>(1) * 0.5f;
//    radius = sqrt(X.at<float>(2) + center.x * center.x + center.y * center.y);
}


std::vector<cv::Point> denoise(std::vector<cv::Point> const& points, cv::Mat const& test) {
    auto const count = points.size();
    for (auto index = 0; index < count; ++index) {
        auto const& point = points[index];

        struct {
            cv::Point2f center, center_tmp;
                 double radius, radius_tmp;
            //
            std::vector<cv::Point> points;
        } arc{};

        for (auto index_next = index; index_next < count; ++index_next) {
            auto const& point_next = points[index_next];

            arc.points.push_back(point_next);
            if (arc.points.size() < 3)
                continue;


            circleFit(arc.points, arc.center_tmp, arc.radius_tmp);
            std::cout << arc.center_tmp << " " << arc.radius_tmp << "\n";
            double s = 0;
            for (auto p : arc.points) {
                auto x = pointDistance2Circle(p, arc.center_tmp, arc.radius_tmp);
                s+=x;
                std::cout << x << " ";
            }
            std::cout << "\n" << s / arc.points.size() <<"\n\n\n";

            auto o = test.clone();
            cv::cvtColor(o, o, cv::COLOR_GRAY2RGB);
            cv::circle(o, arc.center_tmp, arc.radius_tmp, {0, 0, 250}, 3);
            cv::circle(o, point_next, 5, {0, 0, 250}, 3, cv::FILLED);
            show(o, "test");
        }
    }
}

//std::vector<cv::Point> denoise(std::vector<cv::Point> const& points) {
//    auto const count = points.size();
//
//    struct {
//        cv::Point const* s = nullptr;
//        cv::Point const* e = nullptr;
//        double           angle{};
//    } last_line{};
//
//    Arc  * last_arc  = nullptr;
//
//    for (int i = 2; i < count; ++i) {
//        auto const& a = points[i];
//        if (last_line.s) {
//            auto const angle_sa = lineSlope(*last_line.s, a);
//            //
//            if (abs(last_line.angle - angle_sa) < 10) {
//                last_line.e     = &a;
//                last_line.angle = angle_sa;
//                continue;
//            } else {
//            }
//        }
//        auto const& b = points[i + 1 < count ? i + 1 : 0];
//        auto const& c = points[i + 2 < count ? i + 2 : 1];
//
//        if (!last_line.s) {
//            auto const distance_b2ac = pointDistance2Line(b, a, c);
//            if (distance_b2ac < 10) {
//                last_line.s = &a;
//                last_line.e = &c;
//                last_line.angle = lineAngle(a, c);
//            }
//        }
//    }
//}