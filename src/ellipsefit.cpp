/*
 * ellipse_fit.cpp
 * ellipse fittin based on opencv ellipse fit
 * Created on: Jan 29, 2014
 * Author: Ashraf Saleh
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
//	vector<Point> contour;
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
	vector<Point> base_pts;
	vector<Point> contour;
	vector<Point> best_contour;

	RotatedRect box;
	double dist;
	double best_dist = 150*pts.size();
	int inliers_n ; // number of inliers
	float w; // prob of inliers

	int iterations = 0;
	for (int i = 0; i < R; ++i) {
		iterations++;


		// choose n random points
		// every time shuffle the array .. successive shuffles are not bad [[yaoMing]]
		base_pts = choose_random(pts, BASE_POINTS_N);

		// fit ellipse
		box = fit_ellipse(base_pts);

		// find ellipse contour
		contour = ellipse_contour(box);

		// calculate distance and inliers cnt
		dist = distance(contour, pts, inliers_n);
//		cout<<"inliers = "<<inliers_n<<endl;
//		cout<<"dist = "<<dist<<endl;
//		cout<<"best_dist = "<<best_dist<<endl;

		w = 1.0f*inliers_n/pts.size();

//		cout<<"W = "<<w<<endl;


		if (dist < best_dist && w > 0.5){
			best_contour = contour;
			ellipse_bounding_box = box;
			best_dist = dist;
			int new_R =  abs( LOG_1_P / logf(1-pow(w,5)) );
//			cout<<"new R = "<<new_R<<endl;
			R = new_R;
		}

	}
	cout<<"Iterations = "<<iterations<<endl;
	cout<<"=================="<<endl;
	return best_contour;
}
