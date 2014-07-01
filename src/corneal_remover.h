/*
 * corneal_remover.h
 *
 *  Created on: Feb 13, 2014
 *      Author: leo
 */

#ifndef CORNEAL_REMOVER_H_
#define CORNEAL_REMOVER_H_

#include <opencv2/imgproc/imgproc.hpp>
#include "corneal_remover.h"

using namespace std;
using namespace cv;

const int THRESHOLD_TYPE = CV_THRESH_BINARY;
const int MAX_VALUE = 255;
const int MAX_THRESHOLD = 230;
const int THRESHOLD_INCR = 5;
const int CORNEAL_ROI_SIZE = 150; //as mentioned in paper

typedef struct corneal_reflection{
	Point center;
	int radius;
}corneal_reflection;



class corneal_remover{
public:
	Mat remove_corneal_reflection(Mat image, Point start_point);
	corneal_reflection get_corneal_reflection(){
		return cr;
	}
private:
	corneal_reflection cr;
	corneal_reflection locate_corneal_reflection(Mat gray_image, Point center);

};

#endif /* CORNEAL_REMOVER_H_ */
