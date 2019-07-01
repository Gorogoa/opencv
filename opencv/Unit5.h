//MorphoFeatures
#include "opencv2/features2d.hpp"
#pragma once

class MorphoFeatures
{
public:
	MorphoFeatures();
	~MorphoFeatures();
	cv::Mat getConers(const cv::Mat& image);
	void setThreshold(double Val) { threshold = Val; };
	cv::Mat appltThreshold(const cv::Mat& image, const cv::Mat& outputImage);
	vector<cv::Point> returnConers(const cv::Mat& image);
	void drawPoints(const cv::Mat& image, vector<cv::Point> Points);
private:
	double threshold;

	//elements
	cv::Mat_<uchar> cross;
	cv::Mat_<uchar> diamond;
	cv::Mat_<uchar> square;
	cv::Mat_<uchar> x;
};

