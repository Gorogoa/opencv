#include "Unit.h"
#include "Unit4.h"



Histogram::Histogram()
{
	histSize[0] = 256;
	hranges[0] = 0.0;							//							
	hranges[1] = 256.0;							// [0,256)
	ranges[0] = hranges;
	channels[0] = 0;							//处理第一个通道
}


Histogram::~Histogram()
{
}

//------------------------获取直方图------------------//
cv::Mat Histogram::getHistogram(const cv::Mat& image)
{
	cv::Mat hist;

	cv::calcHist(&image,							//							
				1,									//一个图像
				channels,							//要处理的通道
				cv::Mat(),							//无掩码
				hist,								//结果
				1,									//一维直方图
				histSize,							//箱子数量
				ranges,								//像素值的范围
				true,								//是否采用均匀直方图	
				false);								//累加直方图
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

//-------------------------查找表---------------------//
static cv::Mat applyLookUp(const cv::Mat& image, const cv::Mat& lookup)
{
	cv::Mat result;
	//应用查找表
	cv::LUT(image, lookup, result);
	return result;
}
//----------------------------------------------------//

//----------------Stretch Histogram-------------------//
cv::Mat Histogram::stretch(const cv::Mat& image, int minVal)
{
	cv::Mat hist = getHistogram(image);
	int imin = 0;
	for (; imin < histSize[0]; ++imin)
	{
		if (hist.at<float>(imin) > minVal)
					break;
	}
		
	int imax = histSize[0] - 1;
	for (; imax >= 0; --imax)
	{
		if (hist.at<float>(imax) > minVal)
			break;
	}

	int dim = 256;
	//创建查找表
	cv::Mat lookup(1, dim, CV_8U);
	for (int i = 0; i < imin; ++i)
		lookup.at<uchar>(i) = 0;
	for (int i = histSize[0] - 1; i > imax; --i)
		lookup.at<uchar>(i) = 255;
	for (int i = imin; i <= imax; ++i)
		lookup.at<uchar>(i) = cvRound((histSize[0] - 1) * (i - imin) / (imax - imin));
	cv::Mat result;
	result = applyLookUp(image, lookup);
	return result;
}
//----------------------------------------------------//

//----------------------color Reduce------------------//
cv::Mat Histogram::colorReduce(const cv::Mat& image, int div)
{
	int dim = 256;
	//创建查找表
	cv::Mat lookup(1, dim, CV_8U);
	for (int i = 0; i < histSize[0] - 1; i++)
		lookup.at<uchar>(i) = i / div * div + div / 2;
	cv::Mat result = applyLookUp(image, lookup);
	return result;
}
//----------------------------------------------------//

///////////////////////////////////////////////////////////
//----------------color Histogram------------------------//
///////////////////////////////////////////////////////////
colorHistogram::colorHistogram()
{
	//彩色通道的直方图参数设置
	colHistSize[0] = colHistSize[1] = colHistSize[2] = 256;
	//BGR的范围
	colHistRange[0] = 0.0;
	colHistRange[1] = 255.0;
	//所有通道拥有相同的范围
	colRanges[0] = colHistRange;
	colRanges[1] = colHistRange;
	colRanges[2] = colHistRange;
	//三个通道
	colChannels[0] = 0;
	colChannels[1] = 1;
	colChannels[2] = 2;
}

colorHistogram::~colorHistogram()
{}

cv::SparseMat colorHistogram::getColorHistogram(const cv::Mat& image)
{
	cv::SparseMat hist;
	cv::calcHist(&image,					//一个图像
		1,
		colChannels,						//要处理的通道
		cv::Mat(),							//无掩码
		hist,								//结果
		3,									//三维直方图
		colHistSize,						//箱子数量
		colRanges,							//像素值的范围
		true,
		false);				
	
	return hist;
}
void colorHistogram::setSize(const int& size)
{
	colHistSize[0] = colHistSize[1] = colHistSize[2] = size;
}


///////////////////////////////////////////////////////////
//----------------ContentFind----------------------------//
///////////////////////////////////////////////////////////
ContentFind::ContentFind() 
{
	threshold = 0.1f;
}
ContentFind::~ContentFind()
{
}
void ContentFind::setHistogram(const cv::SparseMat& hist)
{
	histogram = hist;
	//cv::normalize(histogram, histogram, 1.0, cv::NormTypes::NORM_MINMAX);							//归一化至0-1.0
}
cv::Mat ContentFind::find(const cv::Mat& image)
{
	int channels[] = { 0,1,2 };
	cv::Mat result;
	float hranges[2] = { 0.0,255.0 };
	const float* ranges[3] = { hranges,hranges ,hranges };
	cv::calcBackProject(&image,								
						1,									//一个图像
						channels,							
						histogram, 
						result, 
						ranges,								//每个维度的范围
						255.0);								//映射至0-255

	//对反向结果进行阈值化处理，
	if (threshold > 0.0)
		cv::threshold(result, result, 255.0 * threshold, 255.0, cv::THRESH_BINARY);							//超过阈值设为255
	return result;
}
int Unit4()
{
//------------------------获取直方图------------------//
	//cv::Mat image = cv::imread("front.jpg",CV_LOAD_IMAGE_COLOR);
	//if (image.empty())
	//	return -1;
	//Histogram h;
	//cv::Mat hist = h.getHistogram(image);
	//cv::Mat histImg = getImageHistogram(hist,2);
	//cv::namedWindow("Histogram");
	//cv::imshow("Histogram",histImg);
//----------------------------------------------------//

//----------------Stretch Histogram-------------------//
	//cv::Mat image = cv::imread("back.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//Histogram h;
	//cv::Mat Stretch = h.stretch(image, 100);
	//cv::Mat hist = h.getHistogram(Stretch);
	//cv::Mat histImg = getImageHistogram(hist,2);
	//cv::namedWindow("Stretch Histogram");
	//cv::imshow("Stretch Histogram", histImg);
//----------------------------------------------------//

//----------------color Reduce------------------------//
	//cv::Mat image = cv::imread("back.jpg", CV_LOAD_IMAGE_COLOR);
	//Histogram h;
	//cv::Mat result = h.colorReduce(image);
	//cv::namedWindow("color Reduce");
	//cv::imshow("color Reduce", result);
//----------------------------------------------------//

//----------------EqualizeHist------------------------//
	//cv::Mat equalizeHistImage = cv::imread("back.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//if (equalizeHistImage.empty())
	//	return -1;
	//cv::Mat equalizeHistResult;
	//cv::equalizeHist(equalizeHistImage, equalizeHistResult);							//直方图均衡化
	//cv::namedWindow("EqualizeHist");
	//cv::imshow("EqualizeHist", equalizeHistResult);
//----------------------------------------------------//

//----------------Content Find------------------------//
	cv::Mat image = cv::imread("back.jpg", CV_LOAD_IMAGE_COLOR);
	if (image.empty())
		return -1;
	cv::Mat imageROI = image(cv::Rect(0, 0, 45, 45));
	colorHistogram h;
	cv::SparseMat colHist;
	h.setSize(128);							//128*128*128
	colHist = h.getColorHistogram(imageROI);

	ContentFind finder;
	finder.setThreshold(0.05f);
	finder.setHistogram(colHist);
	cv::Mat result = finder.find(image);
	cv::rectangle(result, cv::Rect(0, 0, 85, 85), cv::Scalar(0));
	cv::namedWindow("Content Find");
    cv::imshow("Content Find", result);

//----------------------------------------------------//

	return 0;
}