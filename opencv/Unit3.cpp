#include "Unit.h"
#include "Unit3.h"

const int getColorDistance(const cv::Vec3b& color, const cv::Vec3b& teargetColor)  //获取颜色城区距离
{
	/*
	return abs(color[0] - teargetColor[0])
		+ abs(color[1] - teargetColor[1])
		+ abs(color[2] - teargetColor[2]);
	*/
	return static_cast<int>(cv::norm<int, 3>(cv::Vec3i(color[0] - teargetColor[0],
		color[1] - teargetColor[1], color[2] - teargetColor[2])));                 //欧几里德范数
}

void colorDetetor::setTargetColor(uchar blue, uchar green, uchar red)
{
	cv::Mat tmp(1, 1, CV_8UC3);
	cv::cvtColor(cv::Vec3b(blue, green, red), tmp.at<cv::Vec3b>(0, 0), CV_BGR2Lab);//目标颜色转换为Lab空间

	/*target = cv::Vec3b(blue, green, red);*/
	target = tmp.at<cv::Vec3b>(0, 0);
}
void colorDetetor::setColorDistanceThreshold(int distance)
{
	if (distance < 0)
		distance = 0;
	maxDist = distance;
}

cv::Mat colorDetetor::process(const cv::Mat & image)
{
	result.create(image.size(), CV_8U);                                             //与输入图像大小一致的二值图像
	cv::Mat converted(image.size(), image.type());
	cv::cvtColor(image,converted,CV_RGB2Lab);										//转换至Lab空间

	cv::Mat_<cv::Vec3b>::const_iterator iter = converted.begin<cv::Vec3b>();
	auto iterEnd = converted.end<cv::Vec3b>();
	cv::Mat_<uchar>::iterator iterOut = result.begin<uchar>();

	for (; iter != iterEnd; ++iter)
	{
		if (getDistanceToTargetColor(*iter) <= maxDist)
			* iterOut = 0;
		else
		{
			*iterOut = 255;
		}
	}
	return result;
}

//-------------SkinDetect-----------------------------//
void detectHScolor(const cv::Mat& image,
				   double minHue,double maxHue,							//色调区间
				   double minSat,double maxSat,							//饱和度区间
				   cv::Mat& mask)                                       //输出掩码
{
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	vector<cv::Mat> channels;
	cv::split(hsv, channels);

	//色调掩码
	cv::Mat mask_H1;							//小于maxHue 
	cv::threshold(channels[0], mask_H1,maxHue, 255, cv::THRESH_BINARY_INV);
	//cv::THRESH_BINARY_INV:大于maxHue置零,否则为255
	cv::Mat mask_H2;							//大于maxHue 
	cv::threshold(channels[0], mask_H2, minHue, 255, cv::THRESH_BINARY);
	//cv::THRESH_BINARY:大于minHue设为255,否则为0
	cv::Mat hueMask;
	if (minHue < maxHue)
		hueMask = mask_H1 & mask_H2;
	else
		hueMask = mask_H1 | mask_H2;
	
	//饱和度掩码
	cv::Mat mask_S1;							//小于maxSat
	cv::threshold(channels[1], mask_S1, maxSat, 255, cv::THRESH_BINARY_INV);
	cv::Mat mask_S2;							//大于minSat
	cv::threshold(channels[1], mask_S2, minSat, 255, cv::THRESH_BINARY);
	cv::Mat satMask;
	if (minSat < maxSat)
		satMask = mask_S1 & mask_S2;
	else
		satMask = mask_S1 | mask_S2;

	mask = hueMask & satMask;
}	
//----------------------------------------------------//

int  Unit3()
{
	//创建图像处理对象
	colorDetetor cdetect;
	//输入图像
	cv::Mat image = cv::imread("front.jpg");
	if (image.empty())
		return 0;
	//设置参数
	//cdetect.setColorDistanceThreshold(100);
	//cdetect.setTargetColor(0, 0, 0);
	//处理图像并输出	
	//cv::namedWindow("class");
	//cv::imshow("class", cdetect.process(image));

	//cv::Mat hsv;
	//cv::cvtColor(image, hsv, CV_BGR2HSV);							//转换至HSV空间
	//vector <cv::Mat> hsvChannels;
	//cv::split(hsv, hsvChannels);
	//hsvChannels[1] = 255;											//channel[0]:H色调(0-180) channels[1]:S饱和度 channel[2]:V亮度
	//cv::merge(hsvChannels,hsv);
	//cv::Mat imageNew;
	//cv::cvtColor(hsv, imageNew, CV_HSV2BGR);						//转换至RGB空间显示
	//cv::namedWindow("HSV");
	//cv::imshow("HSV", imageNew);

	//肤色检测
	cv::Mat mask;
	detectHScolor(image,160,10,25,166,mask);
	cv::Mat result(image.size(), image.type(), cv::Scalar(0, 0, 0));
	image.copyTo(result, mask);
	cv::namedWindow("SkinDetect");
	cv::imshow("SkinDetect", result);




	cv::waitKey(0);
	return 0;
}