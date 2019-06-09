#pragma once
class Histogram
{
public:
	Histogram();
	~Histogram();

	cv::Mat getHistogram(const cv::Mat& image);

	static cv::Mat getImageHistogram(const cv::Mat& image,int zoom);

private:
	int histSize[1];							//直方图中箱字的数量
	float hranges[2];							//值范围
	const float* ranges[1];						//值范围的指针
	int channels[1];							//要处理的通道

	//彩色
	int colhistSize[3];
	float colhRange[2];
	const float* colRanges[3];						
	int colChannels[3];							
};