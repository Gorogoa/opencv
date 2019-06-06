#include "Unit.h"
#include "Unit3.h"

const int getColorDistance(const cv::Vec3b& color, const cv::Vec3b& teargetColor)  //��ȡ��ɫ��������
{
	/*
	return abs(color[0] - teargetColor[0])
		+ abs(color[1] - teargetColor[1])
		+ abs(color[2] - teargetColor[2]);
	*/
	return static_cast<int>(cv::norm<int, 3>(cv::Vec3i(color[0] - teargetColor[0],
		color[1] - teargetColor[1], color[2] - teargetColor[2])));                 //ŷ����·���
}

void colorDetetor::setTargetColor(uchar blue, uchar green, uchar red)
{
	cv::Mat tmp(1, 1, CV_8UC3);
	cv::cvtColor(cv::Vec3b(blue, green, red), tmp.at<cv::Vec3b>(0, 0), CV_BGR2Lab);//Ŀ����ɫת��ΪLab�ռ�

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
	result.create(image.size(), CV_8U);                                             //������ͼ���Сһ�µĶ�ֵͼ��
	cv::Mat converted(image.size(), image.type());
	cv::cvtColor(image,converted,CV_RGB2Lab);										//ת����Lab�ռ�

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
				   double minHue,double maxHue,							//ɫ������
				   double minSat,double maxSat,							//���Ͷ�����
				   cv::Mat& mask)                                       //�������
{
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	vector<cv::Mat> channels;
	cv::split(hsv, channels);

	//ɫ������
	cv::Mat mask_H1;							//С��maxHue 
	cv::threshold(channels[0], mask_H1,maxHue, 255, cv::THRESH_BINARY_INV);
	//cv::THRESH_BINARY_INV:����maxHue����,����Ϊ255
	cv::Mat mask_H2;							//����maxHue 
	cv::threshold(channels[0], mask_H2, minHue, 255, cv::THRESH_BINARY);
	//cv::THRESH_BINARY:����minHue��Ϊ255,����Ϊ0
	cv::Mat hueMask;
	if (minHue < maxHue)
		hueMask = mask_H1 & mask_H2;
	else
		hueMask = mask_H1 | mask_H2;
	
	//���Ͷ�����
	cv::Mat mask_S1;							//С��maxSat
	cv::threshold(channels[1], mask_S1, maxSat, 255, cv::THRESH_BINARY_INV);
	cv::Mat mask_S2;							//����minSat
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
	//����ͼ�������
	colorDetetor cdetect;
	//����ͼ��
	cv::Mat image = cv::imread("front.jpg");
	if (image.empty())
		return 0;
	//���ò���
	//cdetect.setColorDistanceThreshold(100);
	//cdetect.setTargetColor(0, 0, 0);
	//����ͼ�����	
	//cv::namedWindow("class");
	//cv::imshow("class", cdetect.process(image));

	//cv::Mat hsv;
	//cv::cvtColor(image, hsv, CV_BGR2HSV);							//ת����HSV�ռ�
	//vector <cv::Mat> hsvChannels;
	//cv::split(hsv, hsvChannels);
	//hsvChannels[1] = 255;											//channel[0]:Hɫ��(0-180) channels[1]:S���Ͷ� channel[2]:V����
	//cv::merge(hsvChannels,hsv);
	//cv::Mat imageNew;
	//cv::cvtColor(hsv, imageNew, CV_HSV2BGR);						//ת����RGB�ռ���ʾ
	//cv::namedWindow("HSV");
	//cv::imshow("HSV", imageNew);

	//��ɫ���
	cv::Mat mask;
	detectHScolor(image,160,10,25,166,mask);
	cv::Mat result(image.size(), image.type(), cv::Scalar(0, 0, 0));
	image.copyTo(result, mask);
	cv::namedWindow("SkinDetect");
	cv::imshow("SkinDetect", result);




	cv::waitKey(0);
	return 0;
}