/*
 * RANSAC.cpp
 *
 *  Created on: Jan 29, 2014
 *  Author: Ashraf Saleh
 *
 *  References: book: practical openCV chapter 6. RANSAC algorithm
 *					  http://mathworld.wolfram.com/Ellipse.html
 *  			      http://nicky.vanforeest.com/misc/fitEllipse/fitEllipse.html
 *
 */

#include "RANSAC.h"

RANSAC::RANSAC(int _base_point_cnt, double _p) {
	base_point_cnt = _base_point_cnt;
	p = _p;
}

RANSAC::~RANSAC() {

}

/*
 * pts: vector of potential ellipse points
 * fit ellipse for the given points vector using singular value decomposition
 * http://nicky.vanforeest.com/misc/fitEllipse/fitEllipse.html
 *
 * return a vector of the ellipse conic equation parameters (coefficients)
*/
Mat RANSAC::fit_ellipse(const vector<Point> &pts){
	int N = pts.size();

	Mat D;
	Point p;
	for (int i = 0; i < N; ++i)	{
		p = pts[i];
		D.push_back((Mat_<float>(1, 6)<< p.x*p.x, p.x*p.y, p.y*p.y, p.x, p.y, 1.0f));
	}

	Mat S = D.t()*D;
	Mat S_inv = S.inv();

	Mat C(6, 6, CV_32F);
	C.at<float>(2,0) =2;
	C.at<float>(0,2) =2;
	C.at<float>(1,1) =-1;

	Mat product = S_inv*C;

	Mat evec (6,6,CV_32F);
	Mat eval (1,6,CV_32F);

	cvZero(&evec);
	cvZero(&eval);

	// the book author for some reasons say that this call produces unexpected results
	eigen(product,eval,evec);

	Point max_loc;
	minMaxLoc(eval, NULL, NULL, NULL, &max_loc);

	// the solution is the eigen vector corresponding to the largest eigen value
	return evec.col(max_loc.y);
}

/*
 * the main functions that brings all the parts together and performs the RANSAC algorithm
 * returns a vector of points (best ellipse contour found)
 */
vector<Point> RANSAC::get_ellipse(vector<Point> &pts){
	int R = INF; // maximum number of iterations (lowered at every iteration)
	int base_points_cnt = 6;
	vector<Point> base_pts;
	vector<Point> contour;
	vector<Point> best_contour;

	Mat ellipse_params;
	double dist;
	double best_dist = INF;
	float p = 0.99f;
	int w ; // number of inliers

	for (int i = 0; i < R; ++i) {
		// every time shuffle the array .. successive shuffles are not bad [[yaoMing]]
		base_pts = choose_random(pts, base_points_cnt);
		ellipse_params = fit_ellipse(base_pts);

		// not a valid ellipse
		if (!ellipse_contour(ellipse_params, contour))
			continue;
		dist = distance(contour, pts, w);

		if (dist < best_dist){
			best_contour = contour;
			best_dist = dist;
		}

		int new_R = log(1-p)/log(1-pow(w,5));
		if (new_R < R)
			R = new_R;
	}
	return best_contour;
}
