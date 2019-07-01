#pragma once
class Histogram
{
public:
	Histogram();
	~Histogram();

	cv::Mat getHistogram(const cv::Mat& image);													//获取一维直方图

	//static cv::Mat getImageHistogram(const cv::Mat& image,int zoom);							//获取直方图显示

	//static cv::Mat applyLookUp(const cv::Mat& image, const cv::Mat& lookup);					//查找表
	cv::Mat stretch(const cv::Mat& image, int minVal = 0);                                      //Stretch Histogram
	cv::Mat colorReduce(const cv::Mat& image, int div = 64);									//color reduce
	void setSize(const int& size);													            //设置直方图bin数量

private:
	int histSize[1];							//直方图中箱字的数量
	float hranges[2];							//值范围
	const float* ranges[1];						//值范围的指针
	int channels[1];							//要处理的通道
				
};


class colorHistogram
{
public:
	colorHistogram();
	~colorHistogram();

	cv::SparseMat getColorHistogram(const cv::Mat& image);							//获取三维直方图
	void setSize(const int& size);													//设置直方图bin数量

private:
	int colHistSize[3];
	float colHistRange[2];
	const float* colRanges[3];
	int colChannels[3];
};

class ContentFind
{
private:
	float threshold;
	cv::SparseMat histogram;
public:
	ContentFind();
	~ContentFind();

	void setThreshold(const float& t) { threshold = t; };
	float getThreshodl() { return threshold; };							
	cv::Mat  find(const cv::Mat& image);
	void setHistogram(const cv::SparseMat& hist);							//设置直方图并归一化
};

template <typename T,int N>
class IntegralImage
{
	cv::Mat integralImage;
public:
	IntegralImage(cv::Mat image)
	{
		cv::integral(image, integralImage, cv::DataType<T>::type);
	};
	cv::Vec<T, N> operator()(int x, int y, int width, int height)
	{
		return (integralImage.at<cv::Vec<T, N>>(y + height, x + width)							//右下角
			  - integralImage.at<cv::Vec<T, N>>(y + height, x)									//左下角
			  - integralImage.at<cv::Vec<T, N>>(y, x + width)									//右上角
			  + integralImage.at<cv::Vec<T, N>>(y, x));											//初始点				
	}
};