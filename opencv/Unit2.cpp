#include <../opencv2/core.hpp>
#include <../opencv2/highgui.hpp>
#include <../opencv2/imgproc.hpp>
#include <iostream>
using namespace std;
//---------------------------椒盐噪声------------------------//
void salt(cv::Mat image, int n)
{
	int i, j;
	for (int k = 0;k < n; ++k)
	{
		i = rand() % image.cols;
		j = rand() % image.rows;
		if (image.type() == CV_8UC1)
		{
			image.at<uchar>(j, i) = 255;
			//cv::Mat_<uchar> im(image);
			//im(j, i) = 255;
		}
		else if (image.type() == CV_8UC3)
		{/*
			image.at<cv::Vec3b>(j, i)[0] = 255;
			image.at<cv::Vec3b>(j, i)[1] = 255;
			image.at<cv::Vec3b>(j, i)[2] = 255;
			*/
			cv::Mat_<cv::Vec3b> im(image);
			im(j, i)[0] = 255;
			im(j, i)[1] = 255;
			im(j, i)[2] = 255;
		}

	}
}
//-----------------------------------------------------------//

//---------------------------指针扫描图像--------------------//
void colorReduce(cv::Mat image,int div = 8)
{
	int numRow= image.rows;
	int numElement = image.cols * image.channels();

	for (int j = 0; j < numRow; ++j)
	{
		 uchar* data_in = image.ptr<uchar>(j);							//第i行的地址

		//逐个像素处理
		for (int i = 0; i < numElement; ++i)
		{
			data_in[i] = data_in[i] / div * div + div / 2;                //整数除法
			//data[i] = data[i] - data[i]%div + div/2;                     //取模
		}
	}
	
}
//-----------------------------------------------------------//

//---------------------------对连续图像的高效扫描------------//
void colorReduce(const cv::Mat& image, cv::Mat& result, int div = 8)
{
	result.create(image.rows, image.cols, image.type());
	int numRow = image.rows;
	int numElement = image.cols * image.channels();

	//if(image.step == image.cols * image.elemSize())                       //是否连续 行的有效长？=列数*单个像素大小
	if (image.isContinuous())
	{
		//没有填充像素
		/*
		image.reshape(1,                             //通道数
					1);                              //行数,列数自动计算
		*/
		numElement = numElement * numRow;
		numRow = 1;
	}
	//只需循环一次
	for (int j = 0; j < numRow; ++j)
	{
		const uchar* data_in = image.ptr<uchar>(j);							//第i行的地址
		uchar* data_out = result.ptr<uchar>(j);

		//逐个像素处理
		for (int i = 0; i < numElement; ++i)
		{
			data_out[i] = data_in[i] / div * div + div / 2;                //整数除法
			//data[i] = data[i] - data[i]%div + div/2;                     //取模
		}
	}

}
//-----------------------------------------------------------//

//---------------------------迭代器扫描图像------------------//
void colorReduceIter(cv::Mat image, int div = 8)
{
	//cv::MatIterator_<cv::Vec3b> it;
	//cv::Mat_ <cv::Vec3b>::iterator it;
   	//cv::MatConstIterator_<cv::Vec3b> it;
	//cv::Mat_<cv::Vec3b>::const_iterator it;                               //常量迭代器	
	auto it = image.begin<cv::Vec3b>();                                     //在初始位置获得迭代器
	auto itend = image.end<cv::Vec3b>();

	for (; it != itend; ++it)
	{
		(*it)[0] = (*it)[0] / div * div + div / 2;
		(*it)[1] = (*it)[1] / div * div + div / 2; 
		(*it)[2] = (*it)[2] / div * div + div / 2;
	}
}
//-----------------------------------------------------------//

//--------------------Laplace锐化图像------------------------//
void Sharpen(const cv::Mat& image,cv::Mat& result)
{
	result.create(image.size(), image.type());                                //分配图像数据,如果需要则分配
	int nChannnels = image.channels();

	for (int j = 1; j < image.rows - 1; ++j)
	{
		const uchar* pre_row = image.ptr<uchar>(j - 1);
		const uchar* cur_row = image.ptr<uchar>(j - 1);
		const uchar* next_row = image.ptr<uchar>(j + 1);

		uchar* out_row = result.ptr<uchar>(j);
		for (int i = 1 * nChannnels; i < (image.cols - 1) * nChannnels; ++i)
		{
			out_row[i] = cv::saturate_cast<uchar>(5 * cur_row[i] 
				- cur_row[i+nChannnels]- cur_row[i-nChannnels] 
				- next_row[i]-pre_row[i]);                                     //把结果调整至0~255，,8位无符号整数
		}
	}
	//未处理像素设置为0
	result.row(0).setTo(cv::Scalar(0)); 
	result.row(result.rows - 1).setTo(cv::Scalar(0));
	result.col(0).setTo(cv::Scalar(0));
	result.col(result.cols - 1).setTo(cv::Scalar(0));
}
//-----------------------------------------------------------//

//------------------filter2D---------------------------------//
void Sharpen2D(const cv::Mat& image, cv::Mat& result)
{
	//cv::Mat_<double> kernel = cv::Mat::zeros(cv::Size(3, 3), CV_32F);
	cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));

	kernel.at<float>(1, 1) = 5.0;
	kernel.at<float>(0, 1) = -1.0;
	kernel.at<float>(1, 0) = -1.0;
	kernel.at<float>(1, 2) = -1.0;
	kernel.at<float>(2, 1) = -1.0;

	cv::filter2D(image, result, image.depth(), kernel);
}
//-----------------------------------------------------------//

//------------------图像重映射-------------------------------//
void wave(const cv::Mat& image, cv::Mat& result)
{	
	//映射参数    srcX：储存映射后的列的坐标     srcY：储存映射后的行的坐标
	cv::Mat srcX(image.rows, image.cols,CV_32F);
	cv::Mat srcY(image.rows, image.cols, CV_32F);

	for (int y = 0; y < image.rows;++y)
	{
		for (int x = 0; x < image.cols; ++x)
		{
			srcX.at<float>(y, x) = x;
			srcY.at<float>(y, x) = y + 3*sin(x/2.0);                //原第y行的像素,根据一个sin曲线移动
		}
	}
	cv::remap(image, result, srcX, srcY, cv::INTER_LINEAR);   //映射参数
}
//-----------------------------------------------------------//

int Unit2()
{
	cv::Mat image;
	image = cv::imread("back.jpg");        
	if (image.empty())
	{
		//错误处理
		std::cout << "EMPTY" << std::endl;
		return 0;
	}
	/*
	salt(image, 2000);										    //指向同一数据块
	cv::namedWindow("salt");
	cv::imshow("salt",image);     
	*/

	/*
	colorReduceIter(image, 64);										
	cv::namedWindow("colorReduce");
	cv::imshow("colorReduce", image);
	*/

//------------------图像运算---------------------------------//
	/*
		//c[i] = a[i]+b[i]
		cv::add(imageA,imageB,resultC);
		//c[i] = a[i]+K
		cv::add(imageA,cv::Scale(K),resultC);
		//c[i] = k1*a[i]+k2b[i]
		cv::add(imageA,k1,imageB,k2,resultC);
		//if(mask([i]) c[i] = a[i]+b[i]
		cv::add(imageA,imageB,resultC,mask);
	*/
//-----------------------------------------------------------//

	/*
	cv::Mat out;
	//Sharpen(image, out);
	Sharpen2D(image, out);
	cv::namedWindow("Laplace");
	cv::imshow("Laplace", out);
	*/

//------------------分割图像通道-----------------------------//
	/*
	cv::Mat result;
	cv::Mat imageA = cv::imread("C:\\Users\\character\\Desktop\\B.jpg");
	vector<cv::Mat> planes;                                                        //创建三个图像的向量
	cv::split(imageA, planes);                                                      //分割一个三通道的图像
	cv::merge(planes,result);
	planes[1].setTo(cv::Scalar(0));
	planes[2].setTo(cv::Scalar(0));
	cv::merge(planes,result);
	cv::namedWindow("planes");
	cv::resize(result, result, cv::Size(result.cols/4, result.rows/4));
	cv::imshow("planes", result);
	*/
//-----------------------------------------------------------//

	cv::Mat result;
	wave(image,result);	
	cv::waitKey(0);
	cv::namedWindow("Remap");
	cv::imshow("Remap", result);
	return 0;
}

