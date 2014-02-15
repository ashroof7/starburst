/*
 * SVD.h
 *
 *  Created on: Feb 4, 2014
 *      Author: Norhan
 */

#ifndef SVD_H_
#define SVD_H_



#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <iterator>
#include <stdio.h>
#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* malloc, free, rand */
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;




void  svd(int m, int n, double **a, double **p, double *d, double **q);


#endif /* SVD_H_ */
