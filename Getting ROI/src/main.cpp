#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <sstream>
#define w 1000
int state = 0;
using namespace cv;

using namespace std;
void drawCircle(Mat img, Point center) {
	int thickness = -1;
	int lineType = 8;
	circle(img, center,
	w / 32, Scalar(0, 0, 255), thickness, lineType);
}

int main(int argc, char **argv) {
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
	return 0;
}
