#include "Unit.h"
#include "Unit6.h"

int Unit6()
{
	cv::Mat image = cv::imread("view.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//resize(image,image,cv::Size(image.rows / 2, image.cols / 2));
	//image = 255 - image;							//取反
	if (image.empty())
		return -1;
	cv::Mat result;
//-----------------filter------------------------------//
	//cv::blur(image, result, cv::Size(5, 5));
	//cv::GaussianBlur(image, result, cv::Size(5, 5),1.6);						//gaussian filter
	//cv::medianBlur(image, result, 5);											//中值滤波器
//----------------------------------------------------//

//-----------------sobel--------------------------------//
	cv::Mat sobelX;
	cv::Mat sobelY;	
	//L1模    sobel算子
	//cv::Sobel(image, sobelX, CV_16S, 1, 0,3);
	//cv::Sobel(image, sobelY, CV_16S, 0, 1,3);
	//cv::Sobel(image, sobelY, CV_8U, 0, 1, 3, 0.4, 128);							//x*0.4+128
	//cv::Mat sobel = abs(sobelX) + abs(sobelY);
	//double sobmin, sobmax;
	//cv::minMaxLoc(sobel, &sobmin, &sobmax);
	////sobel = -alpha*sobel + 255;
	//sobel.convertTo(result, CV_8U, -255 / sobmax, 255);

	//L2模   sobel算子
	//cv::Mat norm, dir;
	//double sobmin, sobmax;
	//cv::Sobel(image, sobelX, CV_32F, 1, 0, 3);
	//cv::Sobel(image, sobelY, CV_32F, 0, 1, 3);
	//cv::cartToPolar(sobelX,sobelY,norm,dir);
	//cv::minMaxLoc(norm, &sobmin, &sobmax);
	//norm.convertTo(result, CV_8U, -255 / sobmax, 255);
//----------------------------------------------------//

//-------------------Laplace--------------------------//
	//cv::Mat laplace;							//
	//int     aperture = 3;						//Laplace内核的大小
	//double laplaceMin, laplaceMax;
	//cv::Laplacian(image, laplace, CV_32F, aperture);
	//cv::minMaxLoc(laplace, &laplaceMin, &laplaceMax);
	//laplace.convertTo(result, CV_8U, -255 / laplaceMax, 255);
//----------------------------------------------------//

//-------------------Canny----------------------------//
	cv::Canny(image, result, 175, 350);
//----------------------------------------------------//

	cv::namedWindow("result");
	cv::imshow("result", result);
	return 0;
}