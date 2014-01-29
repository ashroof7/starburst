/*
 * framebuffer.h
 *
 *  Created on: Jan 21, 2014
 *      Author: leo
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>


using namespace std;
using namespace cv;


const int NOISE_KERNEL_LENGTH = 5;
const int NOISE_KERNEL_STD = 2;
const double NOISE_BETA = 0.2;	//hysteresis factor for noise reduction

class framebuffer {
public:
	framebuffer(int camera_index, int width, int height, bool remove_noise);
	virtual ~framebuffer();
	Mat get_next_frame();
	Mat get_last_frame();
	void set_noise_removal(bool value);

private:
	VideoCapture cam_stream;
	Mat last_frame;
	Mat noise_reduction(Mat src);
	bool noise_removal;
	vector<Vec3b> C; // the avg accumulative intensity for all frames

};

#endif /* FRAMEBUFFER_H_ */
