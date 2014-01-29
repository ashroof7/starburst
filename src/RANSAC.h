/*
 * RANSAC.h
 *
 *  Created on: Jan 29, 2014
 *  Author: Ashraf Saleh
 *
 *  References: book: practical openCV chapter 6. RANSAC algorithm
 *					  http://mathworld.wolfram.com/Ellipse.html
 *  			      http://nicky.vanforeest.com/misc/fitEllipse/fitEllipse.html
 *
 */

#ifndef RANSAC_H_
#define RANSAC_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

const double EPS = 10e-6;
const double PI = 3.14159265;
const int INF = 10e6;


class RANSAC{
public:
	RANSAC(int _base_point_cnt, double _p);
	virtual ~RANSAC();
	vector<Point> get_ellipse(vector<Point> &pts);

private:
	int base_point_cnt;
	double p;
	Mat fit_ellipse(const vector<Point> &pts);

	/*
	 * returns a vector of randomly selected n points without replacement
	 * drawn from pts by shuffling the elements of pts.
	 */
	inline vector<Point> choose_random(vector<Point> &pts, int n){
		vector<Point> rnd_pts(n);
		random_shuffle(pts.begin(), pts.end());
		copy(pts.begin(), pts.begin()+n, rnd_pts.begin());
		return rnd_pts;
	}

	/*
	 * generates a contour of an ellipse from the conic equation coefficients
	 * the contour points are inserted in the output parameter contour
	 * returns true of the generated ellipse is valid false otherwise
	 *
	 * using the eqns in (eqn 15 to 23)
	 * http://mathworld.wolfram.com/Ellipse.html
	 */
	inline bool ellipse_contour(const Mat &ellipse_params, vector<Point> &contour){
		float a = ellipse_params.at<float>(0,0);
		float b = ellipse_params.at<float>(1,0)/2;
		float c = ellipse_params.at<float>(2,0);
		float d = ellipse_params.at<float>(3,0)/2;
		float f = ellipse_params.at<float>(4,0)/2;
		float g = ellipse_params.at<float>(5,0);

		if (abs(a*c - b*b)< EPS) // a*c-b*b == 0
			return false;

		Point center((c*d - b*f)/(b*b - a*c), (a*f - b*d)/(b*b - a*c));
		// semi-axis calculations
		float num = 2 * (a*f*f + c*d*d + g*b*b - 2*b*d*f - a*c*g);
		float denum1 = (b*b - a*c) * ( sqrt((a-c)*(a-c) + 4*b*b) - (a + c));
		float denum2 = (b*b - a*c) * (-sqrt((a-c)*(a-c) + 4*b*b) - (a + c));
		float a_semi_axis = sqrt(num / denum1);
		float b_semi_axis = sqrt(num / denum2);
		float major_axis = max(a_semi_axis, b_semi_axis);
		float minor_axis = a_semi_axis + b_semi_axis - major_axis;

		//TODO add another condition (axis related) with thresthold

		// rotation angle phi
		float phi= 0.0f;
		if (abs(b) < EPS) // b == 0
			phi = (a<c) ? 0 : PI/2;
		else
			phi = (a<c) ? (atan2(2*b, a-c)/2):(atan2(2*b, a-c)+PI)/2 ;

		// generate contour points
		int n = 150;
		float dtheta = PI*2/n;
		float theta = 0;
		for (int i = 0; i < n; ++i, theta+=dtheta) {
			Point p(
					center.x + major_axis*cos(theta)*cos(phi) + minor_axis*sin(theta)*sin(phi),
					center.y - major_axis*cos(theta)*sin(phi) + minor_axis*sin(theta)*cos(phi));
			contour.push_back(p);
		}

		return true;
	}

	/*
	 * returns the sum of distance between the ellipse contour and a vector of points,
	 * also sets the output parameter w (number of inliers: number of points that lie on the ellipse contour)
	 */
	inline double distance(const vector<Point> &ellipse_contour, const vector<Point> &pts, int &inliers){
		double dist = 0;
		double temp;
		inliers = 0;
		for (unsigned int i = 0; i < pts.size(); ++i){
			temp = pointPolygonTest(ellipse_contour,pts[i], true);
			if (temp<EPS)
				inliers++;
			dist += temp;
		}
		return dist;
	}

};

#endif /* RANSAC_H_ */
