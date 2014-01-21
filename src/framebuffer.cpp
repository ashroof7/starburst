/*
 * framebuffer.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: leo
 */

#include "framebuffer.h"

framebuffer::framebuffer(int camera_index, int width, int height) {
	cam_stream = VideoCapture(camera_index); // open the default camera

	if(!cam_stream.isOpened()){  // check if we succeeded
		cerr<<"camera is not opened"<<endl;
		return;
	}

	cam_stream.set(CV_CAP_PROP_FRAME_WIDTH, width);
	cam_stream.set(CV_CAP_PROP_FRAME_HEIGHT, height);
}

Mat framebuffer::get_next_frame(){
	Mat frame;
	cam_stream >> frame;
	return frame;
}

framebuffer::~framebuffer() {
}

