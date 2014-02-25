#include <iostream>
#include "framebuffer.h"
#include "ellipsefit.h"
#include "corneal_remover.h"
#include "pupil.h"

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

Mat frame;
Mat no_corneal;
Point center_point;
vector<Point> feature_pts;
vector<Point> ellipse_contour;
vector<vector<Point> > contour_vector;
ellipsefit ef;
vector<Point *> temp;



inline Mat go(Mat &gray_image, Mat &color_image){
	//FIXME urgent find a better estimate for the region of interest for corneal reflection
	corneal_remover cr;
	center_point.x = (gray_image.cols)/2;
	center_point.y = (gray_image.rows)/2;
	no_corneal = cr.remove_corneal_reflection(gray_image, center_point);


	// pupil detection
	pupil p(no_corneal, 18, 10);
	feature_pts.clear();
	temp = p.get_feature_pts();
	for (unsigned int i = 0; i < temp.size(); ++i){
		feature_pts.push_back(Point(temp[i]->x, temp[i]->y));
//		cout<<feature_pts[i]<<endl;
	}

	// ellipse fitting
	ellipse_contour = ef.get_ellipse(feature_pts);
	contour_vector.clear();
	contour_vector.push_back(ellipse_contour);
	drawContours(gray_image, contour_vector, 0, Scalar(0,0,255), 1.5, CV_AA);
	circle(gray_image, Point(FRAME_WIDTH/2, FRAME_HEIGHT/2), 30, Scalar(0,0,255), -1, 8);
	// update the center estimate for the next frame
	center_point = ef.get_bounding_box().center;

	return gray_image;
}


void test_pupil(){
	namedWindow("test_pupil", 1); //Create a window

//	frame = imread("eye_1.png", CV_LOAD_IMAGE_GRAYSCALE);
	frame = imread("eye_2.png", CV_LOAD_IMAGE_GRAYSCALE);

	Mat color_img = imread("eye_2.png", CV_LOAD_IMAGE_COLOR);

	Point p_center = Point(177, 206);

	pupil p(frame, 18, 10);

		feature_pts.clear();
		temp = p.get_feature_pts();
		float x,y;

		circle(color_img, p_center, 2.5, Scalar(255,0,0), -1, 8, 0);
		for (unsigned int i = 0; i < temp.size(); ++i){
			feature_pts.push_back(Point(temp[i]->x, temp[i]->y));
			x = feature_pts[i].x;
			y = feature_pts[i].y;

			line(color_img, Point(x-5, y), Point(x+5, y), Scalar(0,255, 0), 1, 8, 0);
			line(color_img, Point(x, y-5), Point(x, y+5), Scalar(0,255, 0), 1, 8, 0);
			circle(color_img, feature_pts[i], 1, Scalar(0,0,255), -1, 8, 0);

//			cout<<feature_pts[i]<<endl;
		}
	imshow("test_pupil", color_img);
	waitKey(0);
}

int main() {
//	framebuffer framebuff(0, FRAME_WIDTH, FRAME_HEIGHT, 1);

	namedWindow("output", 1); //Create a window
//	while(1){
//		//TODO read gray image directly
//		// get frame after removing noise
//		frame = framebuff.get_next_frame();
//		// change RGB image to gray image
//		cvtColor(frame, gray_image, CV_RGB2GRAY);
//		imshow("output",no_corneal);
//		waitKey(30);
//	}

	frame = imread("eye_1.png", CV_LOAD_IMAGE_GRAYSCALE);
//	frame = imread("eye_2.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat color_img = imread("eye_1.png", CV_LOAD_IMAGE_COLOR);

	go(frame, color_img);
	imshow("output", frame);
	waitKey(0);
}

//	// testing ellipse fitting
//	vector<Point> feature_pts;
//	for (int i = 0; i < 63*2; i+=2){
//		feature_pts.push_back(Point(points[i],points[i+1]));
//	}
//
//	ellipsefit ef;
//	vector<Point> ellipse_contour = ef.get_ellipse(feature_pts);
//
//	Mat img = imread("eye.png");
//
//	//if fail to read the image
//	if ( img.empty() ) {
//		cout << "Error loading the image" << endl;
//		return -1;
//	}
//
//
//
//	namedWindow("namedWindow", 1); //Create a window
//	vector<vector<Point> > contour_vector;
//	contour_vector.push_back(ellipse_contour);
//	drawContours(img, contour_vector, 0, Scalar(0,0,255), 1.5, CV_AA);
//
////	circle(img, Point(FRAME_WIDTH/2, FRAME_HEIGHT/2), 30, Scalar(0,0,255), -1, 8);
//	circle(img, Point(50, 50), 7, Scalar(0,0,255), -1, 8);
//	imshow("namedWindow", img);
//	waitKey(0); // Wait until user press some key
//	return 0;
//
// testing corneal reflection
//	Mat img = imread("eye_1.png", CV_LOAD_IMAGE_GRAYSCALE);
//	corneal_remover cr;
//
//	Mat no_corneal = cr.remove_corneal_reflection(img);
//	namedWindow("no_corneal", 1); //Create a window
//	imshow("no_corneal", no_corneal);
//	waitKey(0); // Wait until user press some key	return 0;

// nourhan's main
//int main() {
//	pupil p("image5.png",18,10);
//	p.test();
//	return 0;
//}

