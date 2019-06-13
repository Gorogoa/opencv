#include "Unit.h"
#include "Unit4.h"



Histogram::Histogram()
{
	histSize[0] = 256;
	hranges[0] = 0.0;							//							
	hranges[1] = 256.0;							// [0,256)
	ranges[0] = hranges;
	channels[0] = 0;							//�����һ��ͨ��
}


Histogram::~Histogram()
{
}

//------------------------��ȡֱ��ͼ------------------//
cv::Mat Histogram::getHistogram(const cv::Mat& image)
{
	cv::Mat hist;

	cv::calcHist(&image,							//							
				1,									//һ��ͼ��
				channels,							//Ҫ�����ͨ��
				cv::Mat(),							//������
				hist,								//���
				1,									//һάֱ��ͼ
				histSize,							//��������
				ranges,								//����ֵ�ķ�Χ
				true,								//�Ƿ���þ���ֱ��ͼ	
				false);								//�ۼ�ֱ��ͼ
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

//-------------------------���ұ�---------------------//
static cv::Mat applyLookUp(const cv::Mat& image, const cv::Mat& lookup)
{
	cv::Mat result;
	//Ӧ�ò��ұ�
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
	//�������ұ�
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
	//�������ұ�
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
	//��ɫͨ����ֱ��ͼ��������
	colHistSize[0] = colHistSize[1] = colHistSize[2] = 256;
	//BGR�ķ�Χ
	colHistRange[0] = 0.0;
	colHistRange[1] = 255.0;
	//����ͨ��ӵ����ͬ�ķ�Χ
	colRanges[0] = colHistRange;
	colRanges[1] = colHistRange;
	colRanges[2] = colHistRange;
	//����ͨ��
	colChannels[0] = 0;
	colChannels[1] = 1;
	colChannels[2] = 2;
}

colorHistogram::~colorHistogram()
{}

cv::SparseMat colorHistogram::getColorHistogram(const cv::Mat& image)
{
	cv::SparseMat hist;
	cv::calcHist(&image,					//һ��ͼ��
		1,
		colChannels,						//Ҫ�����ͨ��
		cv::Mat(),							//������
		hist,								//���
		3,									//��άֱ��ͼ
		colHistSize,						//��������
		colRanges,							//����ֵ�ķ�Χ
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
	//cv::normalize(histogram, histogram, 1.0, cv::NormTypes::NORM_MINMAX);							//��һ����0-1.0
}
cv::Mat ContentFind::find(const cv::Mat& image)
{
	int channels[] = { 0,1,2 };
	cv::Mat result;
	float hranges[2] = { 0.0,255.0 };
	const float* ranges[3] = { hranges,hranges ,hranges };
	cv::calcBackProject(&image,								
						1,									//һ��ͼ��
						channels,							
						histogram, 
						result, 
						ranges,								//ÿ��ά�ȵķ�Χ
						255.0);								//ӳ����0-255

	//�Է�����������ֵ������
	if (threshold > 0.0)
		cv::threshold(result, result, 255.0 * threshold, 255.0, cv::THRESH_BINARY);							//������ֵ��Ϊ255
	return result;
}
int Unit4()
{
//------------------------��ȡֱ��ͼ------------------//
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
	//cv::equalizeHist(equalizeHistImage, equalizeHistResult);							//ֱ��ͼ���⻯
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