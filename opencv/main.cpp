#include <../opencv2/core/core.hpp>
#include <../opencv2/highgui/highgui.hpp>
#include <../opencv2/imgproc.hpp>
#include <iostream>
#include "Unit.h"

using namespace std;

int main()
{
	/*
	cv::Mat image;
	image = cv::imread("front.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat outImage = BilinearInterpolation(image, 0.5, 0.5);
	cv::namedWindow("Zoom");
	cv::imshow("Zoom", outImage);
	*/
	//GradientDescent();
	Unit4();

	cv::waitKey(0);
	return 0;
}