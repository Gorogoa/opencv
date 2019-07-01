#pragma once
#include "Unit.h"

class WaterShedSegmenter
{
private:
	cv::Mat marker;
public:
	WaterShedSegmenter();
	~WaterShedSegmenter();
	void setMarker(const cv::Mat& maeker);
	cv::Mat process(const cv::Mat& image);
	cv::Mat getSegmentation();
};

