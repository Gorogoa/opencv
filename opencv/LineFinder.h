#pragma once
#include "Unit.h"
#define PI 3.1415926

class LineFinder
{
private:
	vector<cv::Vec4i> lines;							//����⵽ֱ�ߵĶ˵������
	
	double deltaRho;									//���Ȳ���
	double deltaTheta;									//�ǶȲ���

	int minVote;										//��СͶƱ��
	double minLength;									//ֱ�ߵ���С����
	double maxGap;										//ֱ��������϶
public:
	LineFinder();
	~LineFinder();
	void setConfig(double dRho = 1.0, double dTheta = PI / 180, int minV = 60, double length = 0.0, double gap = 0.0);
	vector<cv::Vec4i> findLines(const cv::Mat& image);
	void drawHoughLines(cv::Mat& image, cv::Scalar color = cv::Scalar(255, 0, 255));
};

