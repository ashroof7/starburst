/*
 * CenterOfFocus.h
 *
 *  Created on: Mar 5, 2014
 *      Author: motaz
 */

#ifndef CENTEROFFOCUS_H_
#define CENTEROFFOCUS_H_
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <sstream>
#define w 1000

using namespace cv;
using namespace std;

class CenterOfFocus {

public:
	int state;
	pair<Mat, Point> grayImagesAndCenters[5];
	CenterOfFocus();
	void drawCircle(Mat img, Point center);
	int getCOF();
	virtual ~CenterOfFocus();
};

#endif /* CENTEROFFOCUS_H_ */
