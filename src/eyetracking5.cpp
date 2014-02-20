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

int main()
	{
			framebuffer framebuff(0, FRAME_WIDTH, FRAME_HEIGHT, 1);
			Mat frame;

			/*while(1){
				frame = framebuff.get_next_frame();
				imshow("Camera output",frame);
				waitKey(30);
			}

*/

		//Mat img = imread("eye_1.png", CV_LOAD_IMAGE_GRAYSCALE);
		Mat img_,gray11;
		img_=imread("eye.png",1);
		cvtColor(~img_,gray11,CV_BGR2GRAY);
		Point center;
			center.x = (gray11.cols)/2;
						center.y = (gray11.rows)/2;


		corneal_remover cr;

			Mat no_corneal = cr.remove_corneal_reflection(gray11,center);
			imwrite("ex.png",no_corneal);
			namedWindow("no_corneal", 1); //Create a window
			imshow("no_corneal", no_corneal);


		//feature points detection
		pupil p("ex.png",18,10);
		vector<Point *> features = p.test();
		vector<Point> result = getFeatures(features);

		//	// testing ellipse fitting
				vector<Point> feature_pts;

		ellipsefit ef;
		vector<Point> ellipse_contour = ef.get_ellipse(result);


		Mat gray_;
		Mat img = imread("ex.png",1);
		cvtColor(~img,gray_,CV_BGR2GRAY);

	//if fail to read the image
		if ( img.empty() ) {
			cout << "Error loading the image" << endl;
		return -1;
	}



		namedWindow("namedWindow", 1); //Create a window
		//vector<vector<Point> > contour_vector;
		//contour_vector.push_back(ellipse_contour);
		//drawContours(gray_, contour_vector, 0, Scalar(0,0,255), 1.5, CV_AA);

		//circle(gray_, Point(FRAME_WIDTH/2, FRAME_HEIGHT/2), 30, Scalar(0,0,255), -1, 8);
		//circle(gray_, Point(50, 50), 7, Scalar(0,0,255), -1, 8);


		imshow("namedWindow", gray_);

		waitKey(0); // Wait until user press some key
		return 0;









    //ellipse fit
	//ellipsefit ef;
	 //vector<Point> ell= ef.get_ellipse(result);

	 /*namedWindow("namedWindow", 1); //Create a window
	 vector<vector<Point> > contour_vector;
	 contour_vector.push_back(ell);
	 drawContours(image1, contour_vector, 0, Scalar(0,0,255), 1.5, CV_AA);

*/

	 //circle(img, Point(FRAME_WIDTH/2, FRAME_HEIGHT/2), 30, Scalar(0,0,255), -1, 8);
	 //	circle(img, Point(50, 50), 7, Scalar(0,0,255), -1, 8);

	//cvtColor(image1, gray_image, CV_BGR2GRAY);

	//waitKey(0);

	//return 0;
}
