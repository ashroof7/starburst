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
#include "framebuffer.h"
#include "corneal_remover.h"

using namespace cv;
using namespace std;

const int FRAME_WIDTH = 800;
const int FRAME_HEIGHT = 1280;

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

int main() {
	float x, y;

	framebuffer framebuff(0, FRAME_WIDTH, FRAME_HEIGHT, 1);
	Mat frame;

	/*while(1){
	 frame = framebuff.get_next_frame();
	 imshow("Camera output",frame);
	 waitKey(30);
	 }

	 */

	frame = imread("ashraf2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat color_img = imread("ashraf2.jpg", CV_LOAD_IMAGE_COLOR);
	Point center;
		center.x = (frame.cols) / 2;
		center.y = (frame.rows) / 2;


	//if there exist corneal reflection in input image

		 /*corneal_remover cr;

		 Mat no_corneal = cr.remove_corneal_reflection(frame,center);
		 imwrite("ex.png",no_corneal);
		 namedWindow("no_corneal", 1); //Create a window
		 imshow("no_corneal", no_corneal);
*/
			//feature points detection


	pupil p(~frame, 18, 10);




	 vector<Point *> temp = p.test();
	 vector<Point> result = getFeatures(temp);

	 vector<Point> feature_pts;
			 feature_pts.clear();

	for (unsigned int i = 0; i < temp.size(); ++i) {
		feature_pts.push_back(Point(temp[i]->x, temp[i]->y));
	/*	x = feature_pts[i].x;
		y = feature_pts[i].y;

		line(color_img, Point(x - 5, y), Point(x + 5, y), Scalar(0, 255, 0), 1,
				8, 0);
		line(color_img, Point(x, y - 5), Point(x, y + 5), Scalar(0, 255, 0), 1,
				8, 0);
		circle(color_img, feature_pts[i], 2, Scalar(0, 0, 255), -1, 8, 0);
*/
		circle(frame, feature_pts[i], 1, Scalar(255,0,0), 1, 8, 1);

		cout << feature_pts[i] << endl;
	}

	namedWindow("output", 1); //Create a window

	imshow("output", frame);

	//	// testing ellipse fitting

		/*ellipsefit ef;
	 vector<Point> ellipse_contour = ef.get_ellipse(result);


	 Mat gray_;
	 Mat img = imread("ex.png", CV_LOAD_IMAGE_GRAYSCALE);
	 //if fail to read the image
	 if ( img.empty() ) {
	 cout << "Error loading the image" << endl;
	 return -1;
	 }



	// namedWindow("namedWindow", 1); //Create a window
	 vector<vector<Point> > contour_vector;
	 contour_vector.push_back(ellipse_contour);
	 drawContours(img, contour_vector, 0, Scalar(0,0,255), 1.5, CV_AA);

	 circle(img, Point(FRAME_WIDTH/2, FRAME_HEIGHT/2), 30, Scalar(0,0,255), -1, 8);
	 circle(img, Point(50, 50), 7, Scalar(0,0,255), -1, 8);


	 //imshow("namedWindow", img);
*/
	waitKey(0); // Wait until user press some key
	return 0;

	//waitKey(0);

	//return 0;
	/*namedWindow("output", 1); //Create a window
	 frame = imread("eye_2.png", CV_LOAD_IMAGE_GRAYSCALE);
	 pupil p(frame, 18, 10);
	 feature_pts.clear();
	 temp = p.test();
	 for (unsigned int i = 0; i < temp.size(); ++i){
	 feature_pts.push_back(Point(temp[i]->x, temp[i]->y));
	 circle(frame, feature_pts[i], 1, Scalar(0,0,255), 1, 8, 1);
	 cout<<feature_pts[i]<<endl;
	 }


	 imshow("output", frame);*/
}

