#pragma once
class Histogram
{
public:
	Histogram();
	~Histogram();

	cv::Mat getHistogram(const cv::Mat& image);													//��ȡһάֱ��ͼ

	//static cv::Mat getImageHistogram(const cv::Mat& image,int zoom);							//��ȡֱ��ͼ��ʾ

	//static cv::Mat applyLookUp(const cv::Mat& image, const cv::Mat& lookup);					//���ұ�
	cv::Mat stretch(const cv::Mat& image, int minVal = 0);                                      //Stretch Histogram
	cv::Mat colorReduce(const cv::Mat& image, int div = 64);									//color reduce
	void setSize(const int& size);													            //����ֱ��ͼbin����

private:
	int histSize[1];							//ֱ��ͼ�����ֵ�����
	float hranges[2];							//ֵ��Χ
	const float* ranges[1];						//ֵ��Χ��ָ��
	int channels[1];							//Ҫ�����ͨ��
				
};


class colorHistogram
{
public:
	colorHistogram();
	~colorHistogram();

	cv::SparseMat getColorHistogram(const cv::Mat& image);							//��ȡ��άֱ��ͼ
	void setSize(const int& size);													//����ֱ��ͼbin����

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
	void setHistogram(const cv::SparseMat& hist);							//����ֱ��ͼ����һ��
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
		return (integralImage.at<cv::Vec<T, N>>(y + height, x + width)							//���½�
			  - integralImage.at<cv::Vec<T, N>>(y + height, x)									//���½�
			  - integralImage.at<cv::Vec<T, N>>(y, x + width)									//���Ͻ�
			  + integralImage.at<cv::Vec<T, N>>(y, x));											//��ʼ��				
	}
};