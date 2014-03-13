/*
 * corneal_remover.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: Ashraf Saleh
 */


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "corneal_remover.h"


/*
 * The ratio between the area of the largest candidate and the average area of other
 * regions is calculated as the threshold is lowered.
 *
 * the ratio increases as we lower the threshold and at some point it decreases.
 * Hence, at every iteration this ratio increases, once the ratio decreases we found the corneal reflection
 *
 */
corneal_reflection corneal_remover::locate_corneal_reflection(Mat gray_image, Point center){

	corneal_reflection cr;
	cr.radius = -1;


	vector<vector<Point> > contours;

	float prev_ratio = 0;

	float max_area ,area, area_sum;
	int max_area_idx;

	int center_x, center_y, radius; // corneal reflection center_x, center_y, radius

	// setting region of interest
	int sx = max(0, center.x - CORNEAL_ROI_SIZE),
		sy = max(0, center.y - CORNEAL_ROI_SIZE),
		ex = min(gray_image.cols, center.x + CORNEAL_ROI_SIZE),
		ey = min(gray_image.rows, center.y + CORNEAL_ROI_SIZE);

	Mat ROI_image(gray_image, Rect(sx, sy, ex-sx, ey-sy));
	Mat binary_img(ROI_image.rows, ROI_image.cols, CV_8U, 0);


	//TODO use binary search
	for (int thres = MAX_THRESHOLD; thres > 0; --thres) {
		threshold(ROI_image, binary_img, thres, MAX_VALUE, THRESHOLD_TYPE);


		contours.clear();
    	findContours(binary_img, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0, 0) );

		max_area = area_sum = 0;
		for (unsigned int i = 0; i < contours.size(); ++i) {
			area = contourArea(contours[i]);
			area_sum += area;
			if (area> max_area){
				max_area_idx = i;
				max_area = area;
			}
		}

		float ratio = max_area/(area_sum-max_area);
		if (ratio < prev_ratio){
			// corneal reflection found

			// estimating the center of corneal reflection assuming that it's circular
			radius = sqrt(max_area/M_PI);
			center_x = center_y = 0;
			for (unsigned int i = 0; i < contours[max_area_idx].size(); ++i) {
				center_x += contours[max_area_idx][i].x;
				center_y += contours[max_area_idx][i].y;
			}
 			center_x /= (contours[max_area_idx].size());
			center_y /= (contours[max_area_idx].size());

			// adding the offset of the ROI
			cr.center = Point(center_x+sx, center_y+sy);
			cr.radius = radius;

//			cout<<"center "<<cr.center<<" r = "<<cr.radius<<"  area = "<<max_area<<endl;
			break;

			//TODO try fitEllipse instead of estimation
//			RotatedRect rect = fitEllipse(contours[max_area_idx]);
//			return corneal_reflection{rect.center, (rect.size.width+rect.size.height)/2};
		}
		prev_ratio = ratio;
	}

	return cr;
}


/*
 * this implementation is a bit different from the starbrust impl.
 * here we get the average intensity of a points in a ring enclosing the corneal reflection
 * the different part is that we floodfill the corneal reflection with this average intensity
 */
Mat corneal_remover::remove_corneal_reflection(Mat image, Point start_point){
	corneal_reflection cr = locate_corneal_reflection(image, start_point);
	//TODO check
	// assuming that number of border points = perimeter

	//TODO using try and error find optimum margin
	// corneal reflection radius + safety margin
	float r =  cr.radius + 10;
	int n_pts = (int) (M_PI* 2.0 * r);

	int cx = cr.center.x;
	int cy = cr.center.y;
	int x = 0 , y = 0 ;
	int avg_intesity = 0;


	for (int i = 0; i < n_pts; ++i) {
		x = cx + r*sin(i*2.0*M_PI/n_pts);
		y = cy + r*cos(i*2.0*M_PI/n_pts);
		avg_intesity += ( (int) (image.at<char>(y,x)) );
	}

	avg_intesity /= n_pts;

	circle(image, cr.center, r, avg_intesity, -1, 8);
	return image;
}
