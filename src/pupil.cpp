/*
 * pupil.cpp
 *
 *  Created on: Jan 31, 2014
 *      Author: Norhan
 *  this class is based on original starbrust algo implementation
 *
 */

#include "pupil.h"

pupil:: pupil(Mat input,int rays_number,int points){
	N=rays_number;
	image=input;
	minimum_cadidate_features=points;
}


pupil::~pupil() {

}


/*
 * locater the edge points
 * from the rays around the start point, march along each ray to find either edge point or border one
 * return void
 */
void pupil::locate_edge_points(Mat image, int width, int height,
		double cx, double cy,
		int dis, double angle_step, double angle_normal,
		double angle_spread,
		int edge_thresh){

	double angle;
	Point p, *edge;
	double dis_cos, dis_sin;
	int pixel_value1, pixel_value2;


	for (angle = angle_normal - angle_spread / 2 + 0.0001;
			angle < angle_normal + angle_spread / 2; angle += angle_step) {

		dis_cos = dis * cos(angle);
		dis_sin = dis * sin(angle);
		p.x = cx + dis_cos;
		p.y = cy + dis_sin;

		pixel_value1 = image.at<char>( p.y,  p.x);
		while (1) {
			p.x += dis_cos;
			p.y += dis_sin;

			if (p.x < 0 || p.x >= width || p.y < 0 || p.y >= height)
				break;

			pixel_value2 = image.at<char>(p.y, p.x);

			if (pixel_value2 - pixel_value1 > edge_thresh) {
				edge = (Point*) malloc(sizeof(Point));
				edge->x = p.x - dis_cos / 2;
				edge->y = p.y - dis_sin / 2;
				feature_points.push_back(edge);
				edge_intensity_diff.push_back(pixel_value2 - pixel_value1);
				break;
			}
			pixel_value1 = pixel_value2;
		}
	}
}


/*
 * the Starburst Algorithm implementation
 * return void
 */
void pupil::starburst_pupil_contour_detection(Mat gray) {
	std::cout<<"startx="<<start_point.x<<std::endl;
	std::cout<<"starty="<<start_point.y<<std::endl;

	int dis = 7;
	int edge_thresh=pupil_edge_thres;
	double angle_spread = 100 * M_PI / 180;
	int loop_count = 0;
	double angle_step = 2 * M_PI / N;
	double new_angle_step;
	Point *edge, edge_mean;
	double angle_normal;
	double cx = start_point.x;
	double cy = start_point.y;
	int first_ep_num;

//	locate_edge_points(gray, gray.cols, gray.rows, cx, cy, dis,	angle_step, 0, 2*PI, edge_thresh);
//	return;
	while (edge_thresh > 5 && loop_count <= 10) {
		edge_intensity_diff.clear();
		destroy_edge_point();
		while (feature_points.size() < minimum_cadidate_features && edge_thresh > 5 ) {
			edge_intensity_diff.clear();
			destroy_edge_point();
			locate_edge_points(gray, gray.cols, gray.rows, cx, cy, dis,
					angle_step, 0, 2 * M_PI, edge_thresh);
			if (feature_points.size() < minimum_cadidate_features) {
				edge_thresh -= 1;
			}
		}//SECOND WHILE
		if (edge_thresh <= 5) {
			break;
		}

		first_ep_num = feature_points.size();
		for (int i = 0; i < first_ep_num; i++) {
			edge = feature_points.at(i);
			angle_normal = atan2(cy-edge->y, cx-edge->x);
			new_angle_step = angle_step*(edge_thresh*1.0/edge_intensity_diff.at(i));
			locate_edge_points(gray, gray.cols, gray.rows, edge->x, edge->y, dis, new_angle_step, angle_normal,
					angle_spread, edge_thresh);
		}// END FOR


		loop_count += 1;
		edge_mean = get_edge_mean();
		if (fabs(edge_mean.x-cx) + fabs(edge_mean.y-cy) < 10)
			break;

		cx = edge_mean.x;
		cy = edge_mean.y;

	}// FIRST WHILE

	if (loop_count > 10) {
		destroy_edge_point();
		printf("Error! edge points did not converge in %d iterations!\n", loop_count);
		return;
	}

	if (edge_thresh <= 5) {
		destroy_edge_point();
		printf("Error! Adaptive threshold is too low!\n");
		return;
	}

//	std::cout<< "FEATUR POINTS: "<<feature_points.size()<<std::endl;

}//END METHOD


/*
 * return the final vector
 */

vector<Point *> pupil::get_feature_pts(){
	Mat gray = image;
	Mat gray_= ~(image.clone());

	// find best guess to the pupil center
	find_best_guess( gray_);

	starburst_pupil_contour_detection(gray);
	return feature_points;
}



