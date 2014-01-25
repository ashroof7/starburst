#include <iostream>
#include "framebuffer.h"

using namespace std;
using namespace cv;

const int FRAME_WIDTH = 800;
const int FRAME_HEIGHT = 1280;

int main() {
	framebuffer framebuff(0, FRAME_WIDTH, FRAME_HEIGHT, 1);
	Mat frame;

	while(1){
		frame = framebuff.get_next_frame();
		imshow("Camera output",frame);
		waitKey(30);
	}

	return 0;
}
