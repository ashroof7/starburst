#include <iostream>
#include "framebuffer.h"

using namespace std;
using namespace cv;
int main() {
	framebuffer framebuff(0, 600, 800);
	Mat frame;

	while(1){
		frame = framebuff.get_next_frame();
		imshow("Camera output",frame);
		waitKey(10);
	}

	return 0;
}

