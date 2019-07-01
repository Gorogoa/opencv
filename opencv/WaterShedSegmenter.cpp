#include "WaterShedSegmenter.h"

WaterShedSegmenter::WaterShedSegmenter()
{
}


WaterShedSegmenter::~WaterShedSegmenter()
{
}

void WaterShedSegmenter::setMarker(const cv::Mat& markerImage)
{
	markerImage.convertTo(marker, CV_32SC1);							//标记图像:32位有符号整数

}

cv::Mat WaterShedSegmenter::process(const cv::Mat& image)
{
	cv::watershed(image, marker);
	return marker;
}

cv::Mat WaterShedSegmenter::getSegmentation()
{
	cv::Mat tmp;
	//marker.convertTo(tmp,CV_8U,255,255);								//dst=src*scale+shift:将分水岭线条赋值为0
	marker.convertTo(tmp,CV_8U);
	return tmp;
}