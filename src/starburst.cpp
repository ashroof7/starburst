/*
 * starburst.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: Ashraf Saleh
 */

#include "starburst.h"

starburst::starburst(){
}

starburst::~starburst() {
	//TODO free mem
}

Point starburst::go(Mat gray_image){
	//FIXME urgent find a better estimate for the region of interest for corneal reflection
	center_point.x = (gray_image.cols)/2;
	center_point.y = (gray_image.rows)/2;
	gray_image = cr.remove_corneal_reflection(gray_image, center_point);

	cvtColor(gray_image, debug_image, CV_GRAY2RGB);
	// pupil detection
	feature_pts.clear();
	temp = p.get_feature_pts(gray_image);

	if (temp.size() < 50 || temp.size() > 300)
		return center_point;

	float x,y;
	cout<<"temp = "<<temp.size()<<endl;
	feature_pts.clear();
	for (unsigned int i = 0; i < temp.size(); ++i){
		feature_pts.push_back(Point(temp[i]->x, temp[i]->y));
		x = feature_pts[i].x;
		y = feature_pts[i].y;

		line(debug_image, Point(x-5, y), Point(x+5, y), Scalar(0,255, 0), 1, 8, 0);
		line(debug_image, Point(x, y-5), Point(x, y+5), Scalar(0,255, 0), 1, 8, 0);
		circle(debug_image, feature_pts[i], 1, Scalar(0,0,255), -1, 8, 0);
	}

	// ellipse fitting
	cout<<"7a7a 1 "<<feature_pts.size()<<endl;
	ellipse_contour = ef.get_ellipse(feature_pts);
	contour_vector.clear();
	contour_vector.push_back(ellipse_contour);
	drawContours(debug_image, contour_vector, 0, Scalar(0,0,255), 2, CV_AA);
	//	circle(color_image, Point(FRAME_WIDTH/2, FRAME_HEIGHT/2), 30, Scalar(0,0,255), -1, 8);
	// update the center estimate for the next frame
	center_point = ef.get_bounding_box().center;



	x = center_point.x;
	y = center_point.y;

	line(debug_image, Point(x-10, y), Point(x+10, y), Scalar(255, 0, 0), 2, 8, 0);
	line(debug_image, Point(x, y-10), Point(x, y+10), Scalar(255, 0, 0), 2, 8, 0);
	circle(debug_image, center_point, 1.5, Scalar(0, 0, 255), -1, 8, 0);

	return center_point;
}


