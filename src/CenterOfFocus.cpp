/*
 * CenterOfFocus.cpp
 *
 *  Created on: Mar 5, 2014
 *      Author: motaz
 */

#include "CenterOfFocus.h"

CenterOfFocus::CenterOfFocus() {
	state = 0;
}

void CenterOfFocus::drawCircle(Mat img, Point center) {
	int thickness = -1;
	int lineType = 8;
	circle(img, center,
			w / 32, Scalar(0, 0, 255), thickness, lineType);
	circle(img, center,
			w / 40, Scalar(255, 255, 255), thickness, lineType);
	circle(img, center,
			w / 48, Scalar(0, 0, 255), thickness, lineType);
	circle(img, center,
			w / 62, Scalar(255, 255, 255), thickness, lineType);
	circle(img, center,
			w / 79, Scalar(0, 0, 255), thickness, lineType);
}

Point circle_loc[5] ;
void calc_circle_loc(){
//	int w, h; // screen width and height

//	char *command="xrandr | grep '*'";
//	FILE *fpipe = (FILE*)popen(command,"r");
//	char line[256];
//	while ( fgets( line, sizeof(line), fpipe))
//	{
//		printf("%s", line);
//	}
//	close(fpipe);
	// return 0;
}

int CenterOfFocus::getCOF() {
	Mat circleImg = Mat::zeros(w, w, CV_8UC3);
	drawCircle(circleImg, Point( w / 2, w / 2));
	state++;
	namedWindow("firstWindow", CV_WINDOW_NORMAL);
	setWindowProperty("firstWindow", CV_WND_PROP_FULLSCREEN,
			CV_WINDOW_FULLSCREEN);

	VideoCapture cap(0);
	if (!cap.isOpened())
		return -1;

	Mat frame;
	while (true) {
		cap >> frame;
		if (waitKey(15) >= 0) {
			std::ostringstream stm;
			stm << state;
			string fileName = stm.str();
			imwrite(fileName + ".jpg", frame);
			destroyWindow("firstWindow");
			namedWindow("firstWindow", CV_WINDOW_NORMAL);
			setWindowProperty("firstWindow", CV_WND_PROP_FULLSCREEN,
					CV_WINDOW_FULLSCREEN);
			if (state == 1) {
				circleImg = Mat::zeros(w, w, CV_8UC3);
				drawCircle(circleImg, Point((w / 32), (w / 32)));
			} else if (state == 2) {
				circleImg = Mat::zeros(w, w, CV_8UC3);
				drawCircle(circleImg, Point(w - (w / 32), (w / 32)));
			} else if (state == 3) {
				circleImg = Mat::zeros(w, w, CV_8UC3);
				drawCircle(circleImg, Point(w / 32, w - (w / 32)));
			} else if (state == 4) {
				circleImg = Mat::zeros(w, w, CV_8UC3);
				drawCircle(circleImg, Point(w - (w / 32), w - (w / 32)));
			} else {
				destroyWindow("firstWindow");
				cap.release();
				return -1;
			}
			state++;
		}
		imshow("firstWindow", circleImg);
	}
	return 1;
}

CenterOfFocus::~CenterOfFocus() {
}

