/*
 * calibration.cpp
 *
 *  Created on: Feb 9, 2014
 *      Author: Ashraf Saleh
 */

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;


/*
 * S = H.E
 * where S and E are vectors in homogeneous coordinates
 *
 * [Sx]   [a  b  c]   [Ex]
 * [Sy] = [d  e  f] . [Ey]
 * [ 1]   [g  h  i]   [ 1]
 *
 * Sx = a.Ex + b.Ey + c
 * Sx = d.Ex + e.Ey + f
 * Sx = g.Ex + h.Ey + i
 *
 * for a single row [a b c] and using different calibration points
 * Sx1 = a.Ex1 + b.Ey1 + c
 * Sx2 = a.Ex2 + b.Ey2 + c
 * Sx3 = a.Ex3 + b.Ey3 + c
 *
 * the previous equations in matrix form
 * [Sx1]   [Ex1  Ey1  1]   [ a ]
 * [Sx2] = [Ex2  Ey2  1] . [ b ]
 * [Sx3]   [Ex3  Ey3  1]   [ c ]
 *
 *   B = Ax
 */

// Mat a in the sub problem Ax = B ; where x is a single row in H matrix
Matx<float, 3, 3> A;
Vec<float, 3> x;
Vec<float, 3> B;

vector<Point> calib_pts_e; // eye
vector<Point> calib_pts_s; // gaze
Mat H(3, 3, CV_32F);

void find_H(){

	// [a b c] calculation
	for (int i = 0; i < A.rows; ++i) {
			A(i,0) = calib_pts_e[i].x;
			A(i,1) = calib_pts_e[i].y;
			A(i,2) = 1;
			B(i) = calib_pts_s[i].x;
	}
	Vec<float, 3> temp1;
	solve(A, B, temp1);
	H.push_back(temp1);

	// [d e f] calculation
	for (int i = 0; i < A.rows; ++i)
		B(i) = calib_pts_s[i].y;
	Vec<float, 3> temp2;
	solve(A, B, temp2);
	H.push_back(temp2);


	// [g h i] calculation
	for (int i = 0; i < A.rows; ++i)
		B(i) = 1;
	Vec<float, 3> temp3;
	solve(A, B, temp3);
	H.push_back(temp3);


//	cout<<H<<endl;
}




