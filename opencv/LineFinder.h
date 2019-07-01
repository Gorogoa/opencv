#pragma once
#include "Unit.h"
#define PI 3.1415926

class LineFinder
{
private:
	vector<cv::Vec4i> lines;							//被检测到直线的端点的向量
	
	double deltaRho;									//长度步长
	double deltaTheta;									//角度步长

	int minVote;										//最小投票数
	double minLength;									//直线的最小长度
	double maxGap;										//直线上最大空隙
public:
	LineFinder();
	~LineFinder();
	void setConfig(double dRho = 1.0, double dTheta = PI / 180, int minV = 60, double length = 0.0, double gap = 0.0);
	vector<cv::Vec4i> findLines(const cv::Mat& image);
	void drawHoughLines(cv::Mat& image, cv::Scalar color = cv::Scalar(255, 0, 255));
};

