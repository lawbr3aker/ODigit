/*
 *  CurveCSS.cpp
 *  CurveMatching
 *
 *  Created by Roy Shilkrot on 11/28/12.
 *
 */

#include "CurveCSS.h"

#ifdef HAVE_MATHGL
#include <mgl2/mgl.h>
#include <mgl2/window.h>
#endif

#pragma mark CSS image


/* compute curvature of curve after gaussian smoothing 
 from "Shape similarity retrieval under affine transforms", Mokhtarian & Abbasi 2002
 curvex - x position of points
 curvey - y position of points
 kappa - curvature coeff for each point
 sigma - gaussian sigma
 */
void ComputeCurveCSS(const vector<double>& curvex, 
					 const vector<double>& curvey, 
					 vector<double>& kappa, 
					 vector<double>& smoothX, vector<double>& smoothY,
					 double sigma,
					 bool isOpen
					 ) 
{
	int M = round((10.0*sigma+1.0) / 2.0) * 2 - 1;
	assert(M % 2 == 1); //M is an odd number
	
	vector<double> g,dg,d2g; getGaussianDerivs(sigma,M,g,dg,d2g);
	
	vector<double> X,XX,Y,YY;
	getdXcurve(curvex,sigma,smoothX,X,XX,g,dg,d2g,isOpen);
	getdXcurve(curvey,sigma,smoothY,Y,YY,g,dg,d2g,isOpen);
	
	kappa.resize(curvex.size());
	for (int i=0; i<curvex.size(); i++) {
		// Mokhtarian 02' eqn (4)
		kappa[i] = (X[i]*YY[i] - XX[i]*Y[i]) / pow(X[i]*X[i] + Y[i]*Y[i], 1.5);
	}
}

/* find zero crossings on curvature */
vector<int> FindCSSInterestPoints(const vector<double>& kappa) {
	vector<int> crossings;
	for (int i=0; i<kappa.size()-1; i++) {
		if ((kappa[i] < 0 && kappa[i+1] > 0) || kappa[i] > 0 && kappa[i+1] < 0) {
			crossings.push_back(i);
		}
	}
	return crossings;
}

vector<int> EliminateCloseMaximas(const vector<int>& maximasv, map<int,double>& maximas) {
	//eliminate degenerate segments (of very small length)
	vector<int> maximasvv;
	for (int i=0;i<maximasv.size();i++) {
		if (i < maximasv.size()-1 && 
			maximasv[i+1] - maximasv[i] <= 4) 
		{ 
			//segment of small length (1 - 4) - eliminate one point, take largest sigma 
			if (maximas[maximasv[i]] > maximas[maximasv[i+1]]) {
				maximasvv.push_back(maximasv[i]);
			} else {
				maximasvv.push_back(maximasv[i+1]);
			}
			i++; //skip next element as well
		} else {
			maximasvv.push_back(maximasv[i]);
		}
	}
	return maximasvv;
}

/* compute the CSS image */
vector<int> ComputeCSSImageMaximas(const vector<double>& contourx_, const vector<double>& contoury_,
								   vector<double>& contourx, vector<double>& contoury,
								   bool isClosedCurve
								   ) 
{
	ResampleCurve(contourx_, contoury_, contourx, contoury, 200, !isClosedCurve);
	vector<Point2d> pl; PolyLineMerge(pl,contourx,contoury);
	
	map<int,double> maximas;
	
	Mat_<Vec3b> img(500,200,Vec3b(0,0,0)), contourimg(350,350,Vec3b(0,0,0));
	bool done = false;
//#pragma omp parallel for
	for (int i=0; i<490; i++) {
		if (!done) {
			double sigma = 1.0 + ((double)i)*0.1;
			vector<double> kappa, smoothx, smoothy;
			ComputeCurveCSS(contourx, contoury, kappa, smoothx, smoothy,sigma);
			
//			vector<vector<Point> > contours(1);
//			PolyLineMerge(contours[0], smoothx, smoothy);
//			contourimg = Vec3b(0,0,0);
//			drawContours(contourimg, contours, 0, Scalar(255,255,255), CV_FILLED);
			
			vector<int> crossings = FindCSSInterestPoints(kappa);
			if (crossings.size() > 0) {
				for (int c=0; c<crossings.size(); c++) {
					img(i,crossings[c]) = Vec3b(255,0,0);
//					circle(contourimg, contours[0][crossings[c]], 5, Scalar(0,0,255), CV_FILLED);
					
					if (c < crossings.size()-1) {
						if (fabs(crossings[c] - crossings[c+1]) < 5.0) {
							//this is a maxima
							int idx = (crossings[c] + crossings[c+1]) / 2;
//#pragma omp critical
							maximas[idx] = (maximas[idx] < sigma) ? sigma : maximas[idx];
							
							circle(img, Point(idx,i), 1, Scalar(0,0,255), FILLED);
						}
					}
				}
//				char buf[128]; sprintf(buf, "evolution_%05d.png", i);
//				imwrite(buf, contourimg);
//				imshow("evolution", contourimg);
//				waitKey(30);
			} else {
				done = true;
			}

		}
	}
	
	//find largest sigma
	double max_sigma = 0.0;
	for (map<int,double>::iterator itr = maximas.begin(); itr!=maximas.end(); ++itr) {
		if (max_sigma < (*itr).second) {
			max_sigma = (*itr).second;
		}
	}
	//get segments with largest sigma
	vector<int> maximasv;
	for (map<int,double>::iterator itr = maximas.begin(); itr!=maximas.end(); ++itr) {
		if ((*itr).second > max_sigma/8.0) {
			maximasv.push_back((*itr).first);
		}
	}
	//eliminate degenerate segments (of very small length)
	vector<int> maximasvv = EliminateCloseMaximas(maximasv,maximas);	//1st pass
	maximasvv = EliminateCloseMaximas(maximasvv,maximas);				//2nd pass
	maximasv = maximasvv;
	for (vector<int>::iterator itr = maximasv.begin(); itr!=maximasv.end(); ++itr) {
		cout << *itr << " - " << maximas[*itr] << endl;
	}
//	Mat zoom; resize(img,zoom,Size(img.rows*2,img.cols*2));
	imshow("css image",img);
	waitKey();
	return maximasv;
}

#pragma mark Curve Matching

/* calculate the "centroid distance" for the curve */
void GetCurveSignature(const vector<Point2d>& a, vector<double>& signature) {
	signature.resize(a.size());
	Scalar a_mean = mean(a); Point2d a_mpt(a_mean[0],a_mean[1]);
	
	//centroid distance
	for (int i=0; i<a.size(); i++) {
		signature[i] = norm(a[i] - a_mpt);
	}
}

/* from http://paulbourke.net/miscellaneous/correlate/ */
double CalcCrossCorrelation(const vector<double>& x, const vector<double>& y) {
	assert(x.size()==y.size());
	int i,j,n = x.size();
	double mx,my,sx,sy,sxy,denom,r;
	
	/* Calculate the mean of the two series x[], y[] */
	mx = 0;
	my = 0;   
	for (i=0;i<n;i++) {
		mx += x[i];
		my += y[i];
	}
	mx /= n;
	my /= n;
	
	/* Calculate the denominator */
	sx = 0;
	sy = 0;
	for (i=0;i<n;i++) {
		sx += (x[i] - mx) * (x[i] - mx);
		sy += (y[i] - my) * (y[i] - my);
	}
	denom = sqrt(sx*sy);
	
	/* Calculate the correlation series */
//	for (delay=-maxdelay;delay<maxdelay;delay++) 
	int delay = 0;
	{
		sxy = 0;
		for (i=0;i<n;i++) {
			j = i + delay;
			if (j < 0 || j >= n)
				continue;
			else
				sxy += (x[i] - mx) * (y[j] - my);
			/* Or should it be (?)
			 if (j < 0 || j >= n)
			 sxy += (x[i] - mx) * (-my);
			 else
			 sxy += (x[i] - mx) * (y[j] - my);
			 */
		}
		r = sxy / denom;
		
		/* r is the correlation coefficient at "delay" */
	}
	return r;
}

/* calculate the similarity score between two curve segments
 Mai 2010, "Affine-invariant shape matching and recognition under partial occlusion", section 4.1
 */
double MatchTwoSegments(const vector<Point2d>& a_, const vector<Point2d>& b_) {
	assert(a_.size() == b_.size()); //cross correlation will work only for similar length curves
	if(a_.size() <= 1 || b_.size() <= 1) {
		cerr << "degenerate: a_.size() " << a_.size() << " b_.size() " << b_.size() << endl;
		return -1.0; //check degenrate case
	}
	
	vector<double> a_x(a_.size()),a_y(a_.size()),b_x(b_.size()),b_y(b_.size());
	vector<double> a_x_(a_.size()),a_y_(a_.size()),b_x_(b_.size()),b_y_(b_.size());
	vector<Point2d> a = a_, b = b_;
//	PolyLineSplit(a_, a_x_, a_y_); ResampleCurve(a_x_, a_y_, a_x, a_y, 50); PolyLineMerge(a, a_x, a_y);
//	PolyLineSplit(b_, b_x_, b_y_); ResampleCurve(b_x_, b_y_, b_x, b_y, 50); PolyLineMerge(b, b_x, b_y);
	
	Scalar a_mean = mean(a), b_mean = mean(b); 
	Point2d a_mpt(a_mean[0],a_mean[1]),b_mpt(b_mean[0],b_mean[1]);
	vector<Point2d> a_m(a.size()),b_m(b.size());
	for (int i=0; i<a.size(); i++) { a_m[i] = a[i] - a_mpt; }
	for (int i=0; i<b.size(); i++) { b_m[i] = b[i] - b_mpt; }
	
	Mat_<double> a_mM = Mat(a_m).reshape(1).t();
	Mat_<double> b_mM = Mat(b_m).reshape(1).t();
	SVD asvd(a_mM),bsvd(b_mM);
	vector<Point2d> a_canon(a.size()),b_canon(b.size());
	Mat(asvd.vt.t()).copyTo(a_mM); 
	a_mM.reshape(2).copyTo(Mat(a_canon));
	Mat(bsvd.vt.t()).copyTo(b_mM); 
	b_mM.reshape(2).copyTo(Mat(b_canon));

	
	vector<double> a_sig; GetCurveSignature(a_canon, a_sig);
	vector<double> b_sig; GetCurveSignature(b_canon, b_sig);

	double cc = CalcCrossCorrelation(a_sig, b_sig);

#if 0
#ifdef HAVE_MATHGL
	{
		mglGraph gr;
		gr.SubPlot(2, 1, 0, "");
	
		vector<double> a_canon_x,a_canon_y;
		PolyLineSplit(a_canon, a_canon_x, a_canon_y);
		vector<double> b_canon_x,b_canon_y;
		PolyLineSplit(b_canon, b_canon_x, b_canon_y);
		
		mglData mgl_a_x(&(a_canon_x[0]),a_canon_x.size()),mgl_a_y(&(a_canon_y[0]),a_canon_y.size());
		mglData mgl_b_x(&(b_canon_x[0]),b_canon_x.size()),mgl_b_y(&(b_canon_y[0]),b_canon_y.size());
		
		gr.Title("Canonical");
		gr.Aspect(1, 1);	
		gr.SetRanges(-.5, .5, -.5, .5);
		gr.Axis(); 
		gr.Grid();
		gr.Plot(mgl_a_x,mgl_a_y);
		gr.Plot(mgl_b_x,mgl_b_y);	

		
		gr.SubPlot(2, 1, 1, "");
		mglData x(&(a_sig[0]),a_sig.size()),x1(&(b_sig[0]),b_sig.size());
		
		gr.Title("Signature");
		gr.SetRanges(0, max(a_sig.size(),b_sig.size()), 0, 0.55);
		gr.Axis(); 
		gr.Grid();
		gr.Plot(x);
		gr.Plot(x1);

		Mat img(gr.GetHeight(),gr.GetWidth(),CV_8UC3,(void*)gr.GetRGB());
		stringstream ss; ss << "cross correlation " << cc;
		putText(img, ss.str(), Point(10,20), CV_FONT_NORMAL, 1.0, Scalar(255), 2);
		imshow("tmp", img);
		waitKey();
	}	
#endif
#endif
	
	return cc; // > 0.8 ? cc : 0.0;
}

/* match the two curves using adapted Smith-Waterman aligning algorithm
 Mai 2010, "Affine-invariant shape matching and recognition under partial occlusion", section 4.2 */ 

Mat_<double> GetSmithWatermanHMatrix(const vector<vector<Point2d> >& a, const vector<vector<Point2d> >& b) {
	int M = a.size();
	int N = b.size();

	//Smith-Waterman
	Mat_<double> H(M,N-1,0.0);
	for (int i=1; i<M; i++) {
		for (int j=1; j<N-1; j++) {
			vector<double> v(4,0.0); 
			v[1] = H(i-1,j-1) + MatchTwoSegments(a[i], b[j]);
			v[2] = H(i-1,j) - 1.0;
			v[3] = H(i,j-1) - 1.0;
			H(i,j) = *(max_element(v.begin(), v.end()));
		}
	}
	cout << H << endl;
	return H;
}	

/* original Smith Waterman algorithm */
double MatchCurvesSmithWaterman(const vector<vector<Point2d> >& a, const vector<vector<Point2d> >& b, vector<Point>& traceback) 
{	
	Mat_<double> H = GetSmithWatermanHMatrix(a,b);
	Point maxp; double maxval;
	minMaxLoc(H, NULL, &maxval, NULL, &maxp);
	while (H(maxp.y,maxp.x) != 0) {
//				cout << "H(maxp.y-1,maxp.x-1) > H(maxp.y,maxp.x-1)" << H(maxp.y-1,maxp.x-1) << " > " << H(maxp.y,maxp.x-1) << endl;
		if (H(maxp.y-1,maxp.x-1) > H(maxp.y,maxp.x-1) &&
			H(maxp.y-1,maxp.x-1) > H(maxp.y-1,maxp.x)) 
		{
			maxp = maxp - Point(1,1);
			traceback.push_back(maxp);
		} else
		if (H(maxp.y-1,maxp.x) > H(maxp.y-1,maxp.x-1) &&
			H(maxp.y-1,maxp.x) > H(maxp.y,maxp.x-1)) 
		{
			maxp.y--;
			traceback.push_back(maxp);
		} else
		if (H(maxp.y,maxp.x-1) > H(maxp.y-1,maxp.x-1) &&
			H(maxp.y,maxp.x-1) > H(maxp.y-1,maxp.x)) 
		{
			maxp.x--;
			traceback.push_back(maxp);
		}
		else {
			break;
		}
	}
	for (int k=0; k<traceback.size(); k++) {
		cout << traceback[k] << " -> ";
	} 
	cout << endl;
	return maxval;
}

/* adapted Smith Waterman */
double AdaptedMatchCurvesSmithWaterman(const vector<vector<Point2d> >& a, const vector<vector<Point2d> >& b, vector<Point>& traceback) 
{
	int M = a.size();
	int N = b.size();
	
	Mat_<double> H = GetSmithWatermanHMatrix(a,b);

	vector<vector<Point> > tracebacks;
	vector<Point> max_traceback;
	int max_traceback_len = 0;
	for (int i=M-1; i>=2; i--) {
		for (int j=N-2; j>=2; j--) {
			if (i < max_traceback_len || j < max_traceback_len) {
				continue; //skip it, it already can't be longer..
			}
			
			//Traceback
			vector<Point> tmp_traceback;
			Point maxp = Point(i,j);
			tmp_traceback.push_back(maxp);
//			maxp = maxp - Point(1,1);
//			tmp_traceback.push_back(maxp);
			bool movedup = false,movedleft = false;
			while (H(maxp.y,maxp.x) != 0 && maxp.y > 1 && maxp.x > 1) {
				if (H(maxp.y-1,maxp.x-1) > H(maxp.y,maxp.x-1) &&
					H(maxp.y-1,maxp.x-1) > H(maxp.y-1,maxp.x)) 
				{
//					cout << "move left-up" << endl;
					maxp = maxp - Point(1,1);
					traceback.push_back(maxp);
				} else if (H(maxp.y-1,maxp.x) > H(maxp.y-1,maxp.x-1) &&
						H(maxp.y-1,maxp.x) > H(maxp.y,maxp.x-1)) 
				{
//					cout << "move up" << endl;
					maxp.y--;
					movedup = true;
				} else if (H(maxp.y,maxp.x-1) > H(maxp.y-1,maxp.x-1) &&
						H(maxp.y,maxp.x-1) > H(maxp.y-1,maxp.x)) 
				{
//					cout << "move left" << endl;
					maxp.x--;
					movedleft = true;
				}
				if (movedup && movedleft) {
					traceback.push_back(maxp);
					movedup = movedleft = false;
				}
			}
			for (int k=0; k<tmp_traceback.size(); k++) {
				cout << tmp_traceback[k] << " -> ";
			}
			cout << endl;
			if (tmp_traceback.size() > max_traceback_len || 
				(
				 tmp_traceback.size() == max_traceback_len && //if equal - look for highest match
				 H(tmp_traceback.front().y,tmp_traceback.front().x) > H(max_traceback.front().y,max_traceback.front().x)
				)
				) 
			{
				max_traceback_len = tmp_traceback.size();
				max_traceback = tmp_traceback;
				cout << "taking traceback of length " << max_traceback_len << endl;
			}
		}
	}
	
	traceback = max_traceback;
	
	return H(traceback[0].y,traceback[0].x);
}


