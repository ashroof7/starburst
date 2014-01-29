#include <iostream>
#include "framebuffer.h"
#include "RANSAC.h"

using namespace std;
using namespace cv;

const int FRAME_WIDTH = 800;
const int FRAME_HEIGHT = 1280;

int main() {
	framebuffer framebuff(0, FRAME_WIDTH, FRAME_HEIGHT, 1);
	Mat frame;

	vector<Point> potential_pts;
	RANSAC ransac(6, 0.99f);
	vector<Point> ellipse_contour = ransac.get_ellipse(potential_pts);

	while(1){
		frame = framebuff.get_next_frame();
		imshow("Camera output",frame);
		waitKey(30);
	}


	return 0;
}
