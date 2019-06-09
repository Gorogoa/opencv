#pragma once
class Histogram
{
public:
	Histogram();
	~Histogram();

	cv::Mat getHistogram(const cv::Mat& image);

	static cv::Mat getImageHistogram(const cv::Mat& image,int zoom);

private:
	int histSize[1];							//ֱ��ͼ�����ֵ�����
	float hranges[2];							//ֵ��Χ
	const float* ranges[1];						//ֵ��Χ��ָ��
	int channels[1];							//Ҫ�����ͨ��

	//��ɫ
	int colhistSize[3];
	float colhRange[2];
	const float* colRanges[3];						
	int colChannels[3];							
};