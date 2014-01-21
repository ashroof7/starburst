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

class framebuffer {
public:
	framebuffer(int camera_index, int width, int height);
	virtual ~framebuffer();
	Mat get_next_frame();

private:
	VideoCapture cam_stream;

};

#endif /* FRAMEBUFFER_H_ */
