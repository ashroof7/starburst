#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;
using namespace std;
Mat get_homographic_mat(vector<Point2f> eye,vector<Point2f> scene){
	Mat H (3,3,CV_32FC1);

	H=findHomography( eye, scene, CV_RANSAC );
	return H;
}

/** @function main */
//int main( int argc, char** argv )
//{
//  //-- Localize the object
//  std::vector<Point2f> obj (4);
//  std::vector<Point2f> scene (4);
//  obj[0]=Point2f(.1,.2);
//  obj[1]=Point2f(.7,.3);
//  obj[2]=Point2f(.3,.9);
//  obj[3]=Point2f(.8,.9);
//
//
//
//  scene[0]=Point2f(0,0);
//  scene[1]=Point2f(0,1);
//  scene[2]=Point2f(1,0);
//  scene[3]=Point2f(1,1);
//
//
//
//   Mat H = get_homographic_mat( obj, scene );
//for(int i=0;i<H.rows;i++){
//	for(int j=0;j<H.cols;j++){
//		cout<<H.at<float>(i,j)<<"    \t";
//	}
//	cout<<endl;
//}
//
// // perspectiveTransform( obj, scene, H);
//
//
//  return 0;
//  }
