#pragma once
#include "Unit.h"
#include "features2d.hpp"

class MSERFeatures
{
private:
	cv::Ptr<cv::MSER> mser;
	double minAreaRatio;
public:
	MSERFeatures(int minArea = 50,							//�������С�ߴ�
		int maxArea = 14400,								//��������ߴ�			   
		int delta = 5,										//�����ȶ��Եĵ�����ֵ

		double maxVariation = 0.25,							//�����������仯��
		double minDiversity = 0.2							//������͸����������Сֵ
	) {
		mser = cv::MSER::create(delta, minArea, maxArea, maxVariation, minDiversity);
	};
	~MSERFeatures();
	void getBoundingRects(const cv::Mat& image, vector<vector<cv::Point>>& rects);
	cv::Mat drawRectsOnImage(const cv::Mat& image, vector<vector<cv::Point>> rects, cv::Scalar color = 255.0);
};


