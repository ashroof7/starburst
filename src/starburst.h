/*
 * starburst.h
 *
 *  Created on: Mar 13, 2014
 *      Author: Ashraf Saleh
 */

#ifndef STARBURST_H_
#define STARBURST_H_

#include <iostream>
#include "framebuffer.h"
#include "ellipsefit.h"
#include "corneal_remover.h"
#include "pupil.h"

using namespace std;
using namespace cv;

class starburst {

public:
	starburst();
	virtual ~starburst();


	Point go(Mat gray_image);

	inline Mat get_debug_image(){
		return debug_image;
	}

private:
	Point center_point;
	corneal_remover cr;
	ellipsefit ef;
	pupil p;
	Mat debug_image;

	vector<Point> feature_pts;
	vector<Point> ellipse_contour;
	vector<vector<Point> > contour_vector;
	vector<Point *> temp;

};

#endif /* STARBURST_H_ */
