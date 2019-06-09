#include "Unit.h"
#include "Histogram.h"



Histogram::Histogram()
{
	histSize[0] = 256;
	hranges[0] = 0.0;							//							
	hranges[1] = 256.0;							// [0,256)
	ranges[0] = hranges;
	channels[0] = 0;							//处理第一个通道

	//彩色通道的直方图参数设置
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

//------------------------获取直方图------------------//
cv::Mat Histogram::getHistogram(const cv::Mat& image)
{
	cv::Mat hist;

	//cv::calcHist(&image,							//							
	//			1,									//一个图像
	//			channels,							//要处理的通道
	//			cv::Mat(),							//无掩码
	//			hist,								//结果
	//			1,									//一维直方图
	//			histSize,							//箱子数量
	//			ranges);							//像素值的范围
	cv::calcHist(&image,							//							
				1,									//一个图像
				colChannels,						//要处理的通道
				cv::Mat(),							//无掩码
				hist,								//结果
				3,									//三维直方图
				colhistSize,						//箱子数量
				colRanges);							//像素值的范围
	return hist;
}
//----------------------------------------------------//

//------------------画出直方图------------------------//
static cv::Mat getImageHistogram(const cv::Mat& hist, int zoom = 1)
{
	double max = 0;
	double min = 0;
	cv::minMaxLoc(hist, &min, &max, 0,0);

	int histSize = hist.rows;
	cv::Mat histImg(histSize * zoom, histSize * zoom, CV_8U, cv::Scalar(255));
	int hpt = static_cast<int>(0.9 * histSize);							//设置最高点为长度的0.9

	//画出垂直线
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

