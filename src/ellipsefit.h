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

#ifndef ELLIPSEFIT_H_
#define ELLIPSEFIT_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace std;
using namespace cv;

const double EPS = 10e-6;
const double PI = 3.14159265;
const float INLIER_EPS = 1.98;
const int INF = 10e6;


class ellipsefit{
public:
	ellipsefit();
	virtual ~ellipsefit();
	vector<Point> get_ellipse(vector<Point> &pts);

	inline RotatedRect get_bounding_box(){
		return ellipse_bounding_box;
	}
private:
	RotatedRect ellipse_bounding_box;
	RotatedRect fit_ellipse(const vector<Point> &pts);

	/*
	 * returns a vector of randomly selected n points without replacement
	 * drawn from pts by shuffling the elements of pts.
	 */
	inline vector<Point> choose_random(vector<Point> &pts, int n){
		random_shuffle(pts.begin(), pts.end());
		vector<Point> rnd_pts(pts.begin(), pts.begin()+n);
		return rnd_pts;
	}

	/* returns the sum of distance between the ellipse contour and a vector of points,
	 * also sets the output parameter w (number of inliers: number of points that lie on the ellipse contour)
	 */
	inline double distance(const vector<Point> &ellipse_contour, const vector<Point> &pts, int &inliers){
		double dist = 0;
		double temp;
		inliers = 0;
		for (unsigned int i = 0; i < pts.size(); ++i){
			temp = abs(pointPolygonTest(ellipse_contour,pts[i], true));
			if (temp<INLIER_EPS)
				inliers++;
			dist += temp;
		}
		return dist;
	}


	/*
	 * generates a contour of an ellipse from bounding box of the ellipse
	 */
	inline vector<Point> ellipse_contour(const RotatedRect box){
		vector<Point> contour;

		Size2f size = box.size;
		float major_axis = max(size.width, size.height)/2;
		float minor_axis = min(size.width, size.height)/2;
		float phi = box.angle;

//		cout<<major_axis<<" "<<minor_axis<<" "<<phi<<endl;

		// generate contour points
		int n = 150;
		float dtheta = PI*2/n;
		float theta = 0;

		for (int i = 0; i < n; ++i, theta+=dtheta) {
			Point p(
					box.center.x + major_axis*cos(theta)*cos(phi) + minor_axis*sin(theta)*sin(phi),
					box.center.y - major_axis*cos(theta)*sin(phi) + minor_axis*sin(theta)*cos(phi));
			contour.push_back(p);
		}
		return contour;
	}

};

#endif /* ELLIPSEFIT_H_ */
