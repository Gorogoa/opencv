#include "MSERFeatures.h"

MSERFeatures::~MSERFeatures()
{
}

void MSERFeatures::getBoundingRects(const cv::Mat& image, vector<vector<cv::Point>>& rects)
{
	vector<vector<cv::Point>> points;
	vector<cv::Rect> bboxes;
	mser->detectRegions(image, points, bboxes);

	for (auto iter = points.begin(); iter != points.end(); ++iter)
	{
		rects.push_back(*iter);
	}
}

cv::Mat MSERFeatures::drawRectsOnImage(const cv::Mat& image, vector<vector<cv::Point>> rects, cv::Scalar color)
{
	cv::Mat result(image);
	for (auto iter = rects.begin(); iter != rects.end(); ++iter)
	{
		cv::RotatedRect rr = cv::minAreaRect(*iter);
		cv::ellipse(image,rr,cv::Scalar(255));
	}
	return result;
}
