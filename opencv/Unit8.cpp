#include "Unit8.h"

int Unit8()
{
	cv::Mat image = cv::imread("view.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (image.empty())
		return -1;
	cv::Mat result;
//-------------------Harris---------------------------//
	cv::Mat cornerStrength;
	//cv::cornerHarris(image, cornerStrength, 3, 3, 0.01);
	//double threshold = 0.0001;
	//cv::threshold(cornerStrength, result, threshold, 255, cv::THRESH_BINARY);

/*	vector<cv::Point2f> corners;
	cv::goodFeaturesToTrack(image, corners, 250, 0.05, 10);
	for (auto iter = corners.begin(); iter != corners.end(); ++iter)
		cv::circle(image, cv::Point(iter->x,iter->y), 3, cv::Scalar(0), 2)
*/
//----------------------------------------------------//

//-------------------Fast Feature Detector------------//
	//cv::Ptr <cv::FeatureDetector> fast = cv::FastFeatureDetector::create(60);							//fast特征检测器的构造函数
	//vector<cv::KeyPoint> points;
	////检测特征点
	//fast->detect(image,points,cv::Mat());
	//cv::drawKeypoints(image, points, image, cv::Scalar(0), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
//----------------------------------------------------//

//-----------------Brisk------------------------------//
	//cv::Ptr <cv::FeatureDetector> brisk = cv::BRISK::create(140,5);
	//vector<cv::KeyPoint> points;
	//brisk->detect(image,points,cv::Mat());
	//cv::drawKeypoints(image, points, result, cv::Scalar(0), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
//----------------------------------------------------//

//-----------------ORB------------------------------//
	cv::Ptr <cv::FeatureDetector> orb = cv::ORB::create(140,							//关键点数目 
														1.2,							//图层间缩放系数
														8);								//金字塔的图层数量
	vector<cv::KeyPoint> points;
	orb->detect(image, points, cv::Mat());
	cv::drawKeypoints(image, points, result, cv::Scalar(0), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
//----------------------------------------------------//
	cv::imshow("result", result);
	return 0;
}