/*
 * ellipse_fit.cpp
 * ellipse fittin based on opencv ellipse fit
 *  Created on: Jan 29, 2014
 *  Author: Ashraf Saleh
 *
 */

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include "ellipsefit.h"

using namespace std;
using namespace cv;

ellipsefit::ellipsefit() {
}

ellipsefit::~ellipsefit() {

}

/*
 * pts: vector of randomly chosen points
 * fit ellipse for the given points vector using opencv fitellipse
 */
RotatedRect ellipsefit::fit_ellipse(const vector<Point> &pts){
	vector<Point> contour;
	Mat pointsf;
	Mat(pts).convertTo(pointsf, CV_32F);
	return fitEllipse(pointsf);
}

/*
 * the main functions that brings all the parts together and performs the RANSAC algorithm
 * returns a vector of points (best ellipse contour found)
 */
vector<Point> ellipsefit::get_ellipse(vector<Point> &pts){
	if (pts.empty())
		cerr<<"features points is empty !!"<<endl;

	int R = INF; // maximum number of iterations (lowered at every iteration)
	int base_points_cnt = 6; //FIXME change hardcoded value
	vector<Point> base_pts;
	vector<Point> contour;
	vector<Point> best_contour;

	RotatedRect box;
	double dist;
	double best_dist = INF;
	int inliers_n ; // number of inliers
	float p = 0.99f;
	float w; // prob of inliers

	//	R = 1;
	for (int i = 0; i < R; ++i) {

		// every time shuffle the array .. successive shuffles are not bad [[yaoMing]]
		base_pts = choose_random(pts, base_points_cnt);
		box = fit_ellipse(base_pts);
		contour = ellipse_contour(box);
		dist = distance(contour, pts, inliers_n);

		w = 1.0f*inliers_n/pts.size();

		if (dist < best_dist){
			best_contour = contour;
			ellipse_bounding_box = box;
			best_dist = dist;
			int new_R = (int) abs( log(1-p)/log(1-pow(w,5)) );
			R = new_R;
		}

	}
	return best_contour;
}
