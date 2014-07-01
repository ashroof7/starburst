/*
 * framebuffer.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: leo
 */

#include "framebuffer.h"

framebuffer::framebuffer(int camera_index, int width, int height, bool remove_noise) {
	cam_stream = VideoCapture(camera_index); // open the default camera

	if(!cam_stream.isOpened()){  // check if we succeeded
		cerr<<"camera is not opened"<<endl;
		return;
	}

	cam_stream.set(CV_CAP_PROP_FRAME_WIDTH, width);
	cam_stream.set(CV_CAP_PROP_FRAME_HEIGHT, height);

	for (int i = 0; i < height; ++i)
		C.push_back(Vec3b());

	noise_removal = remove_noise;
}

void framebuffer::set_noise_removal(bool value){
	noise_removal = value;
}

Mat framebuffer::get_last_frame(){
	return last_frame;
}

Mat framebuffer::get_next_frame(){
	cam_stream >> last_frame;
	if(noise_removal)
	last_frame = noise_reduction(last_frame);
	return last_frame;
}

inline Mat framebuffer::noise_reduction(Mat src){
	Mat dst(src.rows, src.cols, src.type());

	//shot noise .. applying 5*5 Gaussian filter with STD = 2
	for ( int i = 1; i <= NOISE_KERNEL_LENGTH; i = i + 2 )
		GaussianBlur( src, dst, Size( i, i ), NOISE_KERNEL_STD, NOISE_KERNEL_STD );


	//line noise ..
	double beta2 = 1 - NOISE_BETA;
	Vec3b current_avg; // average line intensity of the current frame
	Vec3b correction; // the correction factor for the current line


	for (int i = 0; i < src.rows; i++) {
		// calculating average line intensity of the current frame
		for (int j = 0; j < src.cols; ++j){
			current_avg += src.at<Vec3b>(i,j);
		}

		current_avg/=src.cols;
		C[i] = current_avg*NOISE_BETA + C[i]*beta2; // update C(i,l)
		correction = C[i] - current_avg;

		for (int j = 0; j < src.cols; ++j)
			dst.at<Vec3b>(i, j) += correction;
	}

	return dst;
}

framebuffer::~framebuffer() {
}

void framebuffer::release_camera() {
	cam_stream.release();
}

