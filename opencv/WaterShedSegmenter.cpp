#include "WaterShedSegmenter.h"

WaterShedSegmenter::WaterShedSegmenter()
{
}


WaterShedSegmenter::~WaterShedSegmenter()
{
}

void WaterShedSegmenter::setMarker(const cv::Mat& markerImage)
{
	markerImage.convertTo(marker, CV_32SC1);							//���ͼ��:32λ�з�������

}

cv::Mat WaterShedSegmenter::process(const cv::Mat& image)
{
	cv::watershed(image, marker);
	return marker;
}

cv::Mat WaterShedSegmenter::getSegmentation()
{
	cv::Mat tmp;
	//marker.convertTo(tmp,CV_8U,255,255);								//dst=src*scale+shift:����ˮ��������ֵΪ0
	marker.convertTo(tmp,CV_8U);
	return tmp;
}