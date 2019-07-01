#include "Unit7.h"

int Unit7()
{
	cv::Mat image = cv::imread("binary image.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (image.empty())
		return -1;
	//cv::Mat result;
//-------------------Hough----------------------------//
	//LineFinder finder;
	//cv::Canny(image, image, 135, 350);
	//finder.setConfig(2, 0.02, 160, 20, 5);
	//vector<cv::Vec4i> lines = finder.findLines(image);
	//cv::cvtColor(image, image, CV_GRAY2BGR);
	//finder.drawHoughLines(image);
//----------------------------------------------------//

//-------------------Find Contours--------------------//
	vector<vector<cv::Point>> contours;
	cv::findContours(image, contours, CV_RETR_EXTERNAL							//检索外部轮廓
									, CV_CHAIN_APPROX_NONE						//每个轮廓的所有像素
	);
	int cmin = 150;
	for (auto iter = contours.begin(); iter != contours.end(); )
	{
		if (iter->size() <= cmin)
			iter = contours.erase(iter);							//指向被删除元素的下一个元素
		else
			++iter;
	}
	std::cout << "Contours " << contours.size() << endl;
	cv::Mat result(image.size(), CV_8U, cv::Scalar(255));
	cv::drawContours(result, contours, -1,							//画出全部轮廓, 
					cv::Scalar(0), 1);
	cv::Rect r0 = cv::boundingRect(contours[0]);					//边界框
	cv::rectangle(result, r0, cv::Scalar(0), 2);

	float radius;
	cv::Point2f center;
	cv::minEnclosingCircle(contours[0], center, radius);			//覆盖圆
	cv::circle(result, center, radius, cv::Scalar(128), 2);

	vector<cv::Point> poly;
	cv::approxPolyDP(contours[0], poly, 7, true);					//多边型逼近
	cv::polylines(result, poly, true, cv::Scalar(64), 2);

	vector<cv::Point> hull;
	cv::convexHull(contours[0], hull);
	cv::polylines(result, hull, true, cv::Scalar(225), 2);			//凸包

	cv::Moments mom =  cv::moments(contours[0]);					//特征矩
	cv::circle(result, cv::Point(mom.m10 / mom.m00, mom.m01 / mom.m00),2,cv::Scalar(0),2);						//计算重心
//----------------------------------------------------//

	cv::imshow("result", result);
	return 0;
}