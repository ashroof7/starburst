#include <iostream>
#include "framebuffer.h"
#include "ellipsefit.h"
#include "corneal_remover.h"
#include "pupil.h"
#include "starburst.h"

using namespace std;
using namespace cv;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
pipeline sb;

void test_pupil(){
	namedWindow("test_pupil", 1); //Create a window

	//	frame = imread("eye_1.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat frame = imread("eye_2.png", CV_LOAD_IMAGE_GRAYSCALE);

	Mat color_img = imread("eye_2.png", CV_LOAD_IMAGE_COLOR);

	Point p_center = Point(177, 206);

	pupil p(18, 10);

	vector<Point> feature_pts;
	vector<Point *> temp = p.get_feature_pts(frame);
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

//void calibrate(){
//	CenterOfFocus calib();
//}


/*
// test camera main
int main(int, char**) {
    VideoCapture cap(1); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    namedWindow("edges",CV_WINDOW_AUTOSIZE);
    int i = 0 ;

    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    cout << "Frame Size = " << dWidth << "x" << dHeight << endl;
    Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

    VideoWriter output("test.avi", CV_FOURCC('P','I','M','1'), 20, frameSize, true);

    Mat frame;
    cap >> frame; // get a new frame from camera
    int k;
    while(1) {
//        cout<<i++<<endl;
    	cap >> frame; // get a new frame from camera
        output.write(frame);
        imshow("edges", frame);
        if( k = waitKey(30) >= 0) {
            cout<<k<<endl;
        	cout<<"esc pressed !"<<endl;
        	break;
        }
    }
    cap.release();
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
*/



int main() {
	framebuffer framebuff(1, FRAME_WIDTH, FRAME_HEIGHT, 0);
	Mat frame, gray_image, result;

	namedWindow("output", 1); //Create a window

	Point center(-1,-1);

	while(1){
		//TODO read gray image directly
		// get frame after removing noise
		frame = framebuff.get_next_frame();
		// change RGB image to gray image
		cvtColor(frame, gray_image, CV_BGR2GRAY);
		flip(gray_image, gray_image, 1);
		if (center.x == -1){ // first time initialization
			center.x = (gray_image.cols)/2;
			center.y = (gray_image.rows)/2;
		}
		center = sb.go(gray_image.clone(), center);


		result = sb.get_debug_image();
		imshow("output",result);
		if(waitKey(100)==27) // ESC pressed
			break;
	}
//	framebuff.release_camera();

	//	Mat frame = imread("eye_1.png", CV_LOAD_IMAGE_GRAYSCALE);
	//	frame = imread("eye_2.png", CV_LOAD_IMAGE_GRAYSCALE);

	//	while(true){
	//		c = sb.go(frame.clone());
	//		cout<<c<<endl;
	//		Mat result = sb.get_debug_image();
	//		imshow("output", result);
	//		waitKey(0);
	//	}
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

