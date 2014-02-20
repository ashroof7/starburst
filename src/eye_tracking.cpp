//============================================================================
// Name        : eye_tracking.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <iostream>
#include "pupil.h"
#include "ellipsefit.h"
#include "corneal_remover.h"

using namespace cv;
using namespace std;

vector<Point> getFeatures(vector<Point *> features) {
	vector<Point> curr;
	int j = 0;
	while (j < features.size()) {
		Point temp;
		cout << j << " \t";
		temp.x = features.front()->x;
		temp.y = features.front()->y;
		features.pop_back();
		//curr.push_back(j);
		curr.push_back(temp);
		j++;
	}
	return curr;
}

//int main() {
//	//corneal reflection
//	Mat gray_image;
//	Mat image1 = imread("eye_1.png", 1);
//	cvtColor(~image1, gray_image, CV_BGR2GRAY);
//	corneal_remover test;
//	Mat afterRemove = test.remove_corneal_reflection(gray_image);
//	imwrite("out.png", afterRemove);
//	namedWindow("corneal_reflection", CV_WINDOW_AUTOSIZE);
//	imshow("corneal_reflection", afterRemove);
//
//
//	//feature points detection
//	pupil p("out.png",18,10);
//	vector<Point *> features = p.test();
//	vector<Point> result = getFeatures(features);
//
//
//
//	//ellipse fit
//	ellipsefit ef;
//	 vector<Point> ell= ef.get_ellipse(result);
//
//	 //namedWindow("namedWindow", 1); //Create a window
//	 //vector<vector<Point> > contour_vector;
//	 //contour_vector.push_back(ell);
//	 //drawContours(image1, contour_vector, 0, Scalar(0,0,255), 1.5, CV_AA);
//
//
//	 //circle(img, Point(FRAME_WIDTH/2, FRAME_HEIGHT/2), 30, Scalar(0,0,255), -1, 8);
//	 //	circle(img, Point(50, 50), 7, Scalar(0,0,255), -1, 8);
//
//	//cvtColor(image1, gray_image, CV_BGR2GRAY);
//
//	waitKey(0);
//
//	return 0;
//}
