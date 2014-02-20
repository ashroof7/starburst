/*
 * pupil.h
 *
 *  Created on: Jan 31, 2014
 *      Author: Norhan
 */

#ifndef PUPIL_H_
#define PUPIL_H_

#include <iostream>
#include <vector>
#include <iterator>
#include <stdio.h>
#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* malloc, free, rand */
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ellipsefit.h"

using namespace std;
using namespace cv;

//#define PI 3.14159265

const int angle_step = 20;    //20 degrees
const int pupil_edge_thres = 20;

class pupil {
public:
	pupil(Mat input, int rays_number, int points);
	virtual ~pupil();
	void starburst_pupil_contour_detection(Mat gray);
	vector<Point *> test();
private:
	Point start_point;
	vector<Point *> feature_points;
	vector<int> edge_intensity_diff;
	int N;
	Mat image,image_;
	int minimum_cadidate_features;

	void locate_edge_points(Mat image, int width, int height, double cx,
			double cy, int dis, double angle_step, double angle_normal,
			double angle_spread, int edge_thresh);

	/*
	 * convert the input image to the gray scale representation
	 * return the converted image
	 */

/*	inline Mat grey_scale_image() {

		Mat gray_image;
		cvtColor(~image, gray_image, CV_BGR2GRAY);

       return gray_image;

	}*/

	/*
	 * count the mean of the current points to use it as the start point in starburst Algorithm next iteration
	 * return the mean point
	 */

	inline Point get_edge_mean() {
		Point *edge;
		int i;
		double sumx = 0, sumy = 0;
		Point edge_mean;
		for (i = 0; i < feature_points.size(); i++) {
			edge = feature_points.at(i);
			sumx += edge->x;
			sumy += edge->y;
		}
		if (feature_points.size() != 0) {
			edge_mean.x = sumx / feature_points.size();
			edge_mean.y = sumy / feature_points.size();
		} else {
			edge_mean.x = -1;
			edge_mean.y = -1;
		}
		return edge_mean;
	}
	/*
	 * delete the point from the vector, if the results do not satisfy the specifications
	 * return void
	 */
	inline void destroy_edge_point() {
		vector<Point*>::iterator iter;

		if (feature_points.size() != 0) {
			for (iter = feature_points.begin(); iter != feature_points.end();
					iter++) {
				free(*iter);
			}
			feature_points.clear();
		}
	}

	/*
	 * guess the best point in pupil contour to consider it as pupil center
	 * return center point
	 * may use the center of image as start point as mentioned in paper
	 */
	inline Point find_best_guess(Mat gray) {
		Point center;
		// Convert to binary image by thresholding it
		threshold(gray, gray, 220, 255, cv::THRESH_BINARY);

		// Find all contours
		vector<std::vector<cv::Point> > contours;
		findContours(gray.clone(), contours, CV_RETR_EXTERNAL,
				CV_CHAIN_APPROX_NONE);

		// Fill holes in each contour

		drawContours(gray, contours, -1, CV_RGB(255,255,255), -1);

		for (int i = 0; i < contours.size(); i++) {
			double area = cv::contourArea(contours[i]);
			Rect rect = cv::boundingRect(contours[i]);
			int radius = rect.width / 2;

			// If contour is big enough and has round shape
			// Then it is the pupil
			if (area >= 30
					&& std::abs(
							1 - ((double) rect.width / (double) rect.height))
							<= 0.2
					&& std::abs(1 - (area / (PI * std::pow(radius, 2))))
							<= 0.2) {
				center.x = rect.x + radius;
				center.y = rect.y + radius;
				start_point.x = rect.x + radius;
				start_point.y = rect.y + radius;

			}
		}
		cout << "cx= " << center.x<< endl;
		cout << "cy= " << center.y << endl;

		return center;

	}

};

#endif /* PUPIL_H_ */
