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

#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include "RANSAC.h"

using namespace std;
using namespace cv;
using namespace Eigen;

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
	cout<<"N = "<<N<<endl;

	Mat D;
	Point p;
	for (int i = 0; i < N; ++i)	{
		p = pts[i];
		Mat row(1, 6, CV_32FC1);
		D.push_back(row = (Mat_<float>(1, 6)<< p.x*p.x, p.x*p.y, p.y*p.y, p.x, p.y, 1.0f));
	}

	Mat S = D.t()*D;

	Mat S_inv = S.inv();
	cout<<"s inv= "<<endl;
	cout<<S_inv<<endl;

	Mat C(6, 6, CV_32FC1);
	C.at<float>(2,0) =2;
	C.at<float>(0,2) =2;
	C.at<float>(1,1) =-1;



	Mat product = S_inv*C;
	cout<<endl<<"product = "<<endl;
	cout<<product<<endl;
//		// eigen values using openCV
//		Mat evec (6,6,CV_32F, 0);
//		Mat eval (1,6,CV_32F, 0);
//		// the book author for some reasons say that this call produces unexpected results
//		eigen(product,eval,evec);
//		Point max_loc;
//		minMaxLoc(eval, NULL, NULL, NULL, &max_loc);
//		return evec.col(max_loc.y);

	// Using EIGEN to calculate eigenvalues and eigenvectors
	Eigen::MatrixXd prod_e;
	cv2eigen(product, prod_e);
	EigenSolver<Eigen::MatrixXd> es(prod_e);

	Mat evec, eval, vec(6, 6, CV_32FC1), val(6, 1, CV_32FC1);

	eigen2cv( es.eigenvectors(), evec);
	eigen2cv( es.eigenvalues(), eval);

	evec.convertTo(evec, CV_32F);
	eval.convertTo(eval, CV_32F);

	// // Eigen returns complex parts in the second channel (which are all 0 here) so select just the first channel
	int from_to[] = {0, 0};
	mixChannels(&evec, 1, &vec, 1, from_to, 1);
	mixChannels(&eval, 1, &val, 1, from_to, 1);

	Point max_loc;
	minMaxLoc(val, NULL, NULL, NULL, &max_loc);
//	 the solution is the eigen vector corresponding to the largest eigen value
	return vec.col(max_loc.y);

}

/*
 * the main functions that brings all the parts together and performs the RANSAC algorithm
 * returns a vector of points (best ellipse contour found)
 */
vector<Point> RANSAC::get_ellipse(vector<Point> &pts){
	if (pts.empty())
		cerr<<"features points is empty !!"<<endl;

	int R = INF; // maximum number of iterations (lowered at every iteration)
	int base_points_cnt = 6; //FIXME change hardcoded value
	vector<Point> base_pts;
	vector<Point> contour;
	vector<Point> best_contour;

	Mat ellipse_params;
	double dist;
	double best_dist = INF;
	float p = 0.99f;
	int w ; // number of inliers

	R = 1;
	for (int i = 0; i < R; ++i) {
		// every time shuffle the array .. successive shuffles are not bad [[yaoMing]]
		base_pts = choose_random(pts, base_points_cnt);


		ellipse_params = fit_ellipse(base_pts);

		cout<<"ellipse_params["<<i<<"] = "<<ellipse_params<<endl;

		// not a valid ellipse
		bool valid =ellipse_contour(ellipse_params, contour);
		cout<<endl;
		if (!valid){
			continue;
		}

		dist = distance(contour, pts, w);
		cout<<"distance = "<<dist<<endl;
		cout<<"inliers = "<<w<<endl;
		if (dist < best_dist){
			cout<<"new best found"<<endl;
			best_contour = contour;
			best_dist = dist;
		}

		int new_R = (int) abs( log(1-p)/log(1-pow(w,5)) );
		cout<<"new R = "<<new_R<<endl;
		if (new_R < R)
			R = new_R;

	}
	return best_contour;
}
