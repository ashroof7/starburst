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

int CenterOfFocus::getCOF() {
	Mat circleImg = Mat::zeros(w, w, CV_8UC3);
	Point center;
	center = Point( w / 2, w / 2);
	drawCircle(circleImg, center);
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
			Mat temp;
			frame.copyTo(temp);
			std::ostringstream stm;
			stm << (state - 1);

			string fileName = stm.str();

			// here we will save the image in array.
			Mat gray_image;
			Point centerOfTarget;
			centerOfTarget.x = center.x;
			centerOfTarget.y = center.y;
			pair<Mat, Point> obj;
			obj.second = centerOfTarget;
			imwrite(fileName + ".jpg", temp);
			cvtColor(temp, gray_image, CV_BGR2GRAY);
			obj.first = gray_image;

			if (state == 1) {
				circleImg = Mat::zeros(w, w, CV_8UC3);
				center = Point((w / 32), (w / 32));
				drawCircle(circleImg, center);
			} else if (state == 2) {
				circleImg = Mat::zeros(w, w, CV_8UC3);
				center = Point(w - (w / 32), (w / 32));
				drawCircle(circleImg, center);
			} else if (state == 3) {
				circleImg = Mat::zeros(w, w, CV_8UC3);
				center = Point(w / 32, w - (w / 32));
				drawCircle(circleImg, center);
			} else if (state == 4) {
				circleImg = Mat::zeros(w, w, CV_8UC3);
				center = Point(w - (w / 32), w - (w / 32));
				drawCircle(circleImg, center);
			} else {
				grayImagesAndCenters[state - 1] = obj;
				destroyWindow("firstWindow");
				cap.release();
				for (int i = 0; i < 5; i++) {
					std::ostringstream no;
					no << i;
					string grayFileName = no.str() + "_gray_image.jpg";

					imwrite(grayFileName, grayImagesAndCenters[i].first);
				}
				break;
			}
			grayImagesAndCenters[state - 1] = obj;
			state++;
		}
		imshow("firstWindow", circleImg);
	}
	return 1;
}

CenterOfFocus::~CenterOfFocus() {
}

