#pragma once
/*
 *  CurveCSS.h
 *  CurveMatching
 *
 *  Created by Roy Shilkrot on 11/28/12.
 *
 */

#pragma mark Curves Utilities

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <fstream>
#include <iostream>

using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/video/video.hpp>

using namespace cv;

template<typename T, typename V>
void PolyLineSplit(const vector<Point_<T> >& pl,vector<V>& contourx, vector<V>& contoury) {
	contourx.resize(pl.size()); 
	contoury.resize(pl.size());
	
	for (int j=0; j<pl.size(); j++) 
	{ 
		contourx[j] = (V)(pl[j].x); 
		contoury[j] = (V)(pl[j].y); 
	}
}

template<typename T, typename V>
void PolyLineMerge(vector<Point_<T> >& pl, const vector<V>& contourx, const vector<V>& contoury) {
	assert(contourx.size()==contoury.size());
	pl.resize(contourx.size());
	for (int j=0; j<contourx.size(); j++) {
		pl[j].x = (T)(contourx[j]);
		pl[j].y = (T)(contoury[j]);
	}
}

template<typename T, typename V>
void ConvertCurve(const vector<Point_<T> >& curve, vector<Point_<V> >& output) {
	output.clear();
	for (int j=0; j<curve.size(); j++) {
		output.push_back(Point_<V>(curve[j].x,curve[j].y));
	}
}

void ResampleCurve(const vector<double>& curvex, const vector<double>& curvey,
				   vector<double>& resampleX, vector<double>& resampleY,
				   int N, bool isOpen = false
				   ) {
	assert(curvex.size()>0 && curvey.size()>0 && curvex.size()==curvey.size());

	vector<Point2d> resamplepl;
    for (int i=0; i<resampleX.size(); ++i)
        resamplepl.emplace_back();
    resamplepl[0].x = curvex[0]; resamplepl[0].y = curvey[0];
	vector<Point2i> pl; PolyLineMerge(pl,curvex,curvey);

	double pl_length = arcLength(pl, false);
	double resample_size = pl_length / (double)N;
	int curr = 0;
	double dist = 0.0;
	for (int i=1; i<N; ) {
		assert(curr < pl.size() - 1);
		double last_dist = norm(pl[curr] - pl[curr+1]);
		dist += last_dist;
//		cout << curr << " and " << curr+1 << "\t\t" << last_dist << " ("<<dist<<")"<<endl;
		if (dist >= resample_size) {
			//put a point on line
			double _d = last_dist - (dist-resample_size);
			Point2d cp(pl[curr].x,pl[curr].y),cp1(pl[curr+1].x,pl[curr+1].y);
			Point2d dirv = cp1-cp; dirv = dirv * (1.0 / norm(dirv));
//			cout << "point " << i << " between " << curr << " and " << curr+1 << " remaining " << dist << endl;
			assert(i < resamplepl.size());
			resamplepl[i] = cp + dirv * _d;
			i++;

			dist = last_dist - _d; //remaining dist

			//if remaining dist to next point needs more sampling... (within some epsilon)
			while (dist - resample_size > 1e-3) {
//				cout << "point " << i << " between " << curr << " and " << curr+1 << " remaining " << dist << endl;
				assert(i < resamplepl.size());
				resamplepl[i] = resamplepl[i-1] + dirv * resample_size;
				dist -= resample_size;
				i++;
			}
		}

		curr++;
	}

	PolyLineSplit(resamplepl,resampleX,resampleY);
}

template<typename T>
void drawOpenCurve(Mat& img, const vector<Point_<T> >& curve, Scalar color, int thickness) {
	vector<Point> curve2i;
	ConvertCurve(curve, curve2i);
	for (int i=0; i<curve2i.size()-1; i++) {
		line(img, curve2i[i], curve2i[i+1], color, thickness);
	}
}

#pragma mark CSS Image

void ComputeCurveCSS(const vector<double>& curvex, 
					 const vector<double>& curvey, 
					 vector<double>& kappa, 
					 vector<double>& smoothX,vector<double>& smoothY,
					 double sigma = 1.0,
					 bool isOpen = false);

vector<int> FindCSSInterestPoints(const vector<double>& kappa);

vector<int> ComputeCSSImageMaximas(const vector<double>& contourx_, const vector<double>& contoury_,
								   vector<double>& contourx, vector<double>& contoury, bool isClosedCurve = true);

template<typename T>
void ComputeCurveCSS(const vector<Point_<T> >& curve, 
					 vector<double>& kappa, 
					 vector<Point_<T> >& smooth,
					 double sigma,
					 bool isOpen = false
					 ) 
{
	vector<double> contourx(curve.size()),contoury(curve.size());
	PolyLineSplit(curve, contourx, contoury);
	
	vector<double> smoothx, smoothy;
	ComputeCurveCSS(contourx, contoury, kappa, smoothx, smoothy, sigma, isOpen);
	
	PolyLineMerge(smooth, smoothx, smoothy);	
}

#pragma mark Curve Segments

template<typename T, typename V>
void GetCurveSegments(const vector<Point_<T> >& curve, const vector<int>& interestPoints, vector<vector<Point_<V> > >& segments, bool closedCurve = true) {
	if (closedCurve) {
		segments.resize(interestPoints.size());
	} else {
		segments.resize(interestPoints.size()+1);
	}

	for (int i = (closedCurve)?0:1; i<segments.size()-1; i++) {
		int intpt_idx = (closedCurve)?i:i-1;
		segments[i].clear();
		for (int j=interestPoints[intpt_idx]; j<interestPoints[intpt_idx+1]; j++) {
			segments[i].push_back(Point_<V>(curve[j].x,curve[j].y));
		}
	}
	if (closedCurve) {
		//put in the segment that passes the 0th point
		segments.back().clear();
		for (int j=interestPoints.back(); j<curve.size(); j++) {
			segments.back().push_back(Point_<V>(curve[j].x,curve[j].y));
		}
		for (int j=0; j<interestPoints[0]; j++) {
			segments.back().push_back(Point_<V>(curve[j].x,curve[j].y));
		}
	} else {
		//put in the segment after the last point
		segments.back().clear();
		for (int j=interestPoints.back(); j<curve.size(); j++) {
			segments.back().push_back(Point_<V>(curve[j].x,curve[j].y));
		}
		//put in the segment before the 1st point
		segments.front().clear();
		for (int j=0; j<interestPoints[0]; j++) {
			segments.front().push_back(Point_<V>(curve[j].x,curve[j].y));
		}
	}
	for (int i=0; i<segments.size(); i++) {
		vector<double> x,y;
		cout <<"segments[i].size() " << segments[i].size() << endl;
		PolyLineSplit(segments[i], x, y); ResampleCurve(x, y, x, y, 50,true); PolyLineMerge(segments[i], x, y);
	}
}
template<typename T, typename V>
void GetCurveSegmentsWithCSSImage(vector<Point_<T> >& curve, vector<int>& interestPoints, vector<vector<Point_<V> > >& segments, bool closedCurve = true) {
	vector<double> contourx(curve.size()),contoury(curve.size());
	PolyLineSplit(curve, contourx, contoury);
	
	vector<double> smoothx, smoothy;
	interestPoints = ComputeCSSImageMaximas(contourx, contoury, smoothx, smoothy);
	
	PolyLineMerge(curve, smoothx, smoothy);
	
	double minx,maxx; minMaxLoc(smoothx, &minx, &maxx);
	double miny,maxy; minMaxLoc(smoothy, &miny, &maxy);
	Mat drawing(maxy,maxx,CV_8UC3,Scalar(0));
	RNG rng(time(NULL));
	Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//	vector<vector<Point_<T> > > contours(1,curve);
//	drawContours( drawing, contours, 0, color, 2, 8);
	drawOpenCurve(drawing, curve, color, 2);
	
	for (int m=0; m<interestPoints.size() ; m++) {
		circle(drawing, curve[interestPoints[m]], 5, Scalar(0,255), FILLED);
	}
	imshow("curve interests", drawing);
	waitKey();
	
	GetCurveSegments(curve, interestPoints, segments, closedCurve);
}

#pragma mark Matching

double MatchTwoSegments(const vector<Point2d>& a, const vector<Point2d>& b);
double MatchCurvesSmithWaterman(const vector<vector<Point2d> >& a, const vector<vector<Point2d> >& b, vector<Point>& traceback); 
double AdaptedMatchCurvesSmithWaterman(const vector<vector<Point2d> >& a, const vector<vector<Point2d> >& b, vector<Point>& traceback);

/* 1st and 2nd derivative of 1D gaussian
 */
void getGaussianDerivs(double sigma, int M, vector<double>& gaussian, vector<double>& dg, vector<double>& d2g) {
	int L = (M - 1) / 2;
	double sigma_sq = sigma * sigma;
	double sigma_quad = sigma_sq*sigma_sq;
	dg.resize(M); d2g.resize(M); gaussian.resize(M);

	Mat_<double> g = getGaussianKernel(M, sigma, CV_64F);
	for (double i = -L; i < L+1.0; i += 1.0) {
		int idx = (int)(i+L);
		gaussian[idx] = g(idx);
		// from http://www.cedar.buffalo.edu/~srihari/CSE555/Normal2.pdf
		dg[idx] = (-i/sigma_sq) * g(idx);
		d2g[idx] = (-sigma_sq + i*i)/sigma_quad * g(idx);
	}
}

/* 1st and 2nd derivative of smoothed curve point */
void getdX(vector<double> x,
		   int n,
		   double sigma,
		   double& gx,
		   double& dgx,
		   double& d2gx,
		   vector<double> g,
		   vector<double> dg,
		   vector<double> d2g,
		   bool isOpen = false)
{
	int L = (g.size() - 1) / 2;

	gx = dgx = d2gx = 0.0;
//	cout << "Point " << n << ": ";
	for (int k = -L; k < L+1; k++) {
		double x_n_k;
		if (n-k < 0) {
			if (isOpen) {
				//open curve - mirror values on border
				x_n_k = x[-(n-k)];
			} else {
				//closed curve - take values from end of curve
				x_n_k = x[x.size()+(n-k)];
			}
		} else if(n-k > x.size()-1) {
			if (isOpen) {
				//mirror value on border
				x_n_k = x[n+k];
			} else {
				x_n_k = x[(n-k)-(x.size())];
			}
		} else {
//			cout << n-k;
			x_n_k = x[n-k];
		}
//		cout << "* g[" << g[k + L] << "], ";

		gx += x_n_k * g[k + L]; //gaussians go [0 -> M-1]
		dgx += x_n_k * dg[k + L];
		d2gx += x_n_k * d2g[k + L];
	}
//	cout << endl;
}


/* 0th, 1st and 2nd derivatives of whole smoothed curve */
void getdXcurve(vector<double> x,
				double sigma,
				vector<double>& gx,
				vector<double>& dx,
				vector<double>& d2x,
				vector<double> g,
				vector<double> dg,
				vector<double> d2g,
				bool isOpen = false)
{
	gx.resize(x.size());
	dx.resize(x.size());
	d2x.resize(x.size());
	for (int i=0; i<x.size(); i++) {
		double gausx,dgx,d2gx; getdX(x,i,sigma,gausx,dgx,d2gx,g,dg,d2g,isOpen);
		gx[i] = gausx;
		dx[i] = dgx;
		d2x[i] = d2gx;
	}
}