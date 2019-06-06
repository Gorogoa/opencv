#pragma once
#include "Unit.h"

const int getColorDistance(const cv::Vec3b& color, const cv::Vec3b& teargetColor);  //��ȡ��ɫ��������

class colorDetetor
{
	                         
public:
	colorDetetor() :maxDist(100), target(0, 0, 0) {};							   //���캯��

	void setTargetColor(uchar blue, uchar green, uchar red);                       //����Ŀ������ɫ
	cv::Vec3b GetTargetColor() const { return target; };                           //��ȡĿ������ɫ

	void setColorDistanceThreshold(int distance);                                  //������ɫ�����ֵ
	int  getColorDistanceThreshold() const { return maxDist; };                    //��ȡ��ɫ�����ֵ

	int getDistanceToTargetColor(const cv::Vec3b& color) const { return getColorDistance(color, target); };
	cv::Mat process(const cv::Mat& image);
private:
	int maxDist;
	cv::Vec3b target;
	cv::Mat result;
};