#pragma once
#include "Unit.h"
#include "features2d.hpp"

class MSERFeatures
{
private:
	cv::Ptr<cv::MSER> mser;
	double minAreaRatio;
public:
	MSERFeatures(int minArea = 50,							//允许的最小尺寸
		int maxArea = 14400,								//允许的最大尺寸			   
		int delta = 5,										//测量稳定性的的增量值

		double maxVariation = 0.25,							//最大允许面积变化量
		double minDiversity = 0.2							//子区域和父区域差距的最小值
	) {
		mser = cv::MSER::create(delta, minArea, maxArea, maxVariation, minDiversity);
	};
	~MSERFeatures();
	void getBoundingRects(const cv::Mat& image, vector<vector<cv::Point>>& rects);
	cv::Mat drawRectsOnImage(const cv::Mat& image, vector<vector<cv::Point>> rects, cv::Scalar color = 255.0);
};


