#include "LineFinder.h"

LineFinder::LineFinder():deltaRho(1),deltaTheta(PI/180),minVote(10),minLength(0.0),maxGap(0.0)
{
}


LineFinder::~LineFinder()
{
}

void LineFinder::setConfig(double dRho, double dTheta, int minV, double length, double gap)
{
	deltaRho = dRho;
	deltaTheta = dTheta;
	minVote = minV;
	minLength = length;
	maxGap = gap;
}

vector<cv::Vec4i> LineFinder::findLines(const cv::Mat& image)
{
	lines.clear();
	cv::HoughLinesP(image, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);
	return lines;
}

void LineFinder::drawHoughLines(cv::Mat& image, cv::Scalar color)
{
	for (auto iter = lines.begin(); iter != lines.end(); ++iter)
	{
		cv::Point point1((*iter)[0], (*iter)[1]);
		cv::Point point2((*iter)[2], (*iter)[3]);
		cv::line(image, point1, point2, color);
	}
}
