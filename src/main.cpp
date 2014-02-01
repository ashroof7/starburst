#include <iostream>
#include "framebuffer.h"
#include "ellipsefit.h"

using namespace std;
using namespace cv;

const int FRAME_WIDTH = 800;
const int FRAME_HEIGHT = 1280;


int points[] = {
		//test
		242, 221,
		190, 252,
		137, 217,
		136, 160,
		195, 130,
		247, 163,
		//inliers
		143, 225,
		140, 221,
		139, 216,
		128, 199,
		135, 195,
		132, 188,
		133, 181,
		139, 161,
		144, 161,
		144, 159,
		148, 150,
		154, 147,
		163, 134,
		177, 130,
		219, 134,
		223, 138,
		235, 141,
		241, 154,
		241, 158,
		249, 163,
		249, 168,
		253, 178,
		255, 191,
		255, 195,
		251, 205,
		249, 209,
		245, 215,
		243, 219,
		239, 226,
		235, 231,
		224, 237,
		218, 242,
		211, 242,
		200, 249,
		191, 251,
		183, 252,
		181, 249,
		//outliers
		88, 274,
		94, 272,
		142, 296,
		157, 291,
		167, 301,
		200, 305,
		222, 302,
		215, 336,
		261, 305,
		305, 163,
		360, 186,
		222, 150,
		256, 107,
		224, 100,
		201, 116,
		197, 121,
		180, 111,
		166, 120,
		150, 110,
		150, 91,
		69, 104
};



int main() {
	//	framebuffer framebuff(0, FRAME_WIDTH, FRAME_HEIGHT, 1);
	//	Mat frame;

	//	vector<Point> potential_pts;
	//	RANSAC ransac(6, 0.99f);
	//	vector<Point> ellipse_contour = ransac.get_ellipse(potential_pts);

	//	while(1){
	//		frame = framebuff.get_next_frame();
	//		imshow("Camera output",frame);
	//		waitKey(30);
	//	}


	// testing ellipse fitting
	vector<Point> feature_pts;
	for (int i = 0; i < 63*2; i+=2){
		feature_pts.push_back(Point(points[i],points[i+1]));
	}

	ellipsefit ef;
	vector<Point> ellipse_contour = ef.get_ellipse(feature_pts);

	Mat img = imread("eye.png");

	//if fail to read the image
	if ( img.empty() ) {
		cout << "Error loading the image" << endl;
		return -1;
	}


	//Create a window
	namedWindow("namedWindow", 1);
	//show the image
	// Wait until user press some key
	vector<vector<Point> > contour_vector;
	contour_vector.push_back(ellipse_contour);
	drawContours(img, contour_vector, 0, Scalar(0,0,255), 1.5, CV_AA);
	imshow("namedWindow", img);
	waitKey(0);


	return 0;
}
