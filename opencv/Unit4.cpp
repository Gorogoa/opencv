#include "Unit.h"
#include "Histogram.h"



Histogram::Histogram()
{
	histSize[0] = 256;
	hranges[0] = 0.0;							//							
	hranges[1] = 256.0;							// [0,256)
	ranges[0] = hranges;
	channels[0] = 0;							//�����һ��ͨ��

	//��ɫͨ����ֱ��ͼ��������
	colhistSize[0] = colhistSize[1] = colhistSize[2] = 256;
	colhRange[0] = 0.0;
	colhRange[1] = 256.0;
	colRanges[0] = colhRange;
	colRanges[1] = colhRange;
	colRanges[2] = colhRange;
	colChannels[0] = 0;
	colChannels[1] = 1;
	colChannels[2] = 2;
}


Histogram::~Histogram()
{
}

//------------------------��ȡֱ��ͼ------------------//
cv::Mat Histogram::getHistogram(const cv::Mat& image)
{
	cv::Mat hist;

	//cv::calcHist(&image,							//							
	//			1,									//һ��ͼ��
	//			channels,							//Ҫ�����ͨ��
	//			cv::Mat(),							//������
	//			hist,								//���
	//			1,									//һάֱ��ͼ
	//			histSize,							//��������
	//			ranges);							//����ֵ�ķ�Χ
	cv::calcHist(&image,							//							
				1,									//һ��ͼ��
				colChannels,						//Ҫ�����ͨ��
				cv::Mat(),							//������
				hist,								//���
				3,									//��άֱ��ͼ
				colhistSize,						//��������
				colRanges);							//����ֵ�ķ�Χ
	return hist;
}
//----------------------------------------------------//

//------------------����ֱ��ͼ------------------------//
static cv::Mat getImageHistogram(const cv::Mat& hist, int zoom = 1)
{
	double max = 0;
	double min = 0;
	cv::minMaxLoc(hist, &min, &max, 0,0);

	int histSize = hist.rows;
	cv::Mat histImg(histSize * zoom, histSize * zoom, CV_8U, cv::Scalar(255));
	int hpt = static_cast<int>(0.9 * histSize);							//������ߵ�Ϊ���ȵ�0.9

	//������ֱ��
	for (int h = 0; h < histSize; h++)
	{
		float binVal = hist.at<float>(h);
		if (binVal > 0)
		{
			int intensity = static_cast<int>(binVal * hpt / max);
			cv::line(histImg, cv::Point(h * zoom, histSize * zoom),
				cv::Point(h * zoom, (histSize - intensity) * zoom),
				cv::Scalar(0), zoom);
		}
	}
	return histImg;
}
//----------------------------------------------------//

int Unit4()
{
	cv::Mat image = cv::imread("front.jpg",CV_LOAD_IMAGE_COLOR);
	if (image.empty())
		return -1;
	Histogram h;
	cv::Mat hist = h.getHistogram(image);
	cv::Mat histImg = getImageHistogram(hist(0),2);

	cv::namedWindow("Histogram");
	cv::imshow("Histogram",histImg);
	return 0;
}

