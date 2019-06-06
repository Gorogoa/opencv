#pragma once
#include "Unit.h"

const int getColorDistance(const cv::Vec3b& color, const cv::Vec3b& teargetColor);  //获取颜色城区距离

class colorDetetor
{
	                         
public:
	colorDetetor() :maxDist(100), target(0, 0, 0) {};							   //构造函数

	void setTargetColor(uchar blue, uchar green, uchar red);                       //设置目标检测颜色
	cv::Vec3b GetTargetColor() const { return target; };                           //获取目标检测颜色

	void setColorDistanceThreshold(int distance);                                  //设置颜色差距阈值
	int  getColorDistanceThreshold() const { return maxDist; };                    //获取颜色差距阈值

	int getDistanceToTargetColor(const cv::Vec3b& color) const { return getColorDistance(color, target); };
	cv::Mat process(const cv::Mat& image);
private:
	int maxDist;
	cv::Vec3b target;
	cv::Mat result;
};