/*
 * pipeline.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: Ashraf Saleh
 */

#include "pipeline.h"

pipeline::pipeline(){
}

pipeline::~pipeline() {
	//TODO free mem
}


Rect adhoc_correction(Mat gray_image, Point& center){
	// setting region of interest
	int sx = max(0, center.x - CORNEAL_ROI_SIZE),
			sy = max(0, center.y - CORNEAL_ROI_SIZE),
			ex = min(gray_image.cols, center.x + CORNEAL_ROI_SIZE),
			ey = min(gray_image.rows, center.y + CORNEAL_ROI_SIZE);

	Mat ROI_image(gray_image, Rect(sx, sy, ex-sx, ey-sy));

	Mat binary_img;
	threshold(ROI_image, binary_img, 90, MAX_VALUE, CV_THRESH_BINARY_INV);


	// dilation
	int dilation_size = 18;
	Mat dilate_element = getStructuringElement( MORPH_ELLIPSE,
			Size( 2*dilation_size + 1, 2*dilation_size+1 ),
			Point( dilation_size, dilation_size ) );
	dilate( binary_img, binary_img, dilate_element );

	//	 erosion
	int erosion_size = 25;
	Mat erode_element = getStructuringElement( MORPH_ELLIPSE,
			Size( 2*erosion_size + 1, 2*erosion_size+1 ),
			Point( erosion_size, erosion_size ) );

	// erode 2 times
	erode( binary_img, binary_img, erode_element );
	erode( binary_img, binary_img, erode_element );
	dilate( binary_img, binary_img, erode_element );


	// may use edge to vote on an ellipse model
	//	Mat edges;
	//	double th1 = 300, th2 = 100;
	//	Canny(binary_img, edges, th1, th2, 3, false);
	//
	//	imshow("ROI", ROI_image);
	//	imshow("soso", ~binary_img);
	//	imshow("edges", edges);

	vector<vector<Point> > contours; // Vector for storing contour
	vector<Vec4i> hierarchy;

	findContours( binary_img, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image
	double largest_area = 0;
	int largest_contour_index = -1;
	Rect bounding_rect;

	for(unsigned int i = 0; i< contours.size(); i++ ) {
		double area = contourArea( contours[i], false);
		if( area > largest_area){
			largest_area = area;
			largest_contour_index=i;
		}
	}

	if (largest_contour_index >=0)
		bounding_rect = boundingRect(contours[largest_contour_index]); // Find the bounding rectangle
	//TODO figure out what to return if the image contains no pupil


	// making the rectangle a square
	int side = min(bounding_rect.width, bounding_rect.height);
	bounding_rect.width = side;
	bounding_rect.height = side;

	//shift the bounding rectangle with the ROI displacement (shift to the whole image scale)
	bounding_rect.x+=sx;
	bounding_rect.y+=sy;

	// setting the center point
	center.x = bounding_rect.x + side/2;
	center.y = bounding_rect.y + side/2;



	return bounding_rect;
}

Point pipeline::go(Mat gray_image, Point center){
	center_point = center;
	debug_image = gray_image;

	// center_point is set inside adhoc_correction
	Rect rec= adhoc_correction(gray_image, center_point);
	int x = center.x;
	int y = center.y;

	cvtColor(gray_image, debug_image, CV_GRAY2RGB);
	//	rectangle(debug_image, rec, Scalar(0, 0, 255), 2, 8, 0);
	line(debug_image, Point(x-10, y), Point(x+10, y), Scalar(255, 0, 0), 2, 8, 0);
	line(debug_image, Point(x, y-10), Point(x, y+10), Scalar(255, 0, 0), 2, 8, 0);
	circle(debug_image, center_point, rec.width/2, Scalar(0, 255, 0), 2, 8, 0);

	return center_point;
}

Point pipeline::go_starburst(Mat gray_image, Point center){
	center_point = center;
	debug_image = gray_image;
	gray_image = cr.remove_corneal_reflection(gray_image, center_point);
	center_point = cr.get_corneal_reflection().center;

	//	pupil detection
	feature_pts.clear();
	temp = p.get_feature_pts(gray_image);

	if (temp.size() < 50 || temp.size() > 300)
		return center_point;

	//	if (temp.size() < 50)
	//		return center_point;

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


