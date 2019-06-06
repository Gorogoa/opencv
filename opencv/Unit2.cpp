#include <../opencv2/core.hpp>
#include <../opencv2/highgui.hpp>
#include <../opencv2/imgproc.hpp>
#include <iostream>
using namespace std;
//---------------------------��������------------------------//
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

//---------------------------ָ��ɨ��ͼ��--------------------//
void colorReduce(cv::Mat image,int div = 8)
{
	int numRow= image.rows;
	int numElement = image.cols * image.channels();

	for (int j = 0; j < numRow; ++j)
	{
		 uchar* data_in = image.ptr<uchar>(j);							//��i�еĵ�ַ

		//������ش���
		for (int i = 0; i < numElement; ++i)
		{
			data_in[i] = data_in[i] / div * div + div / 2;                //��������
			//data[i] = data[i] - data[i]%div + div/2;                     //ȡģ
		}
	}
	
}
//-----------------------------------------------------------//

//---------------------------������ͼ��ĸ�Чɨ��------------//
void colorReduce(const cv::Mat& image, cv::Mat& result, int div = 8)
{
	result.create(image.rows, image.cols, image.type());
	int numRow = image.rows;
	int numElement = image.cols * image.channels();

	//if(image.step == image.cols * image.elemSize())                       //�Ƿ����� �е���Ч����=����*�������ش�С
	if (image.isContinuous())
	{
		//û���������
		/*
		image.reshape(1,                             //ͨ����
					1);                              //����,�����Զ�����
		*/
		numElement = numElement * numRow;
		numRow = 1;
	}
	//ֻ��ѭ��һ��
	for (int j = 0; j < numRow; ++j)
	{
		const uchar* data_in = image.ptr<uchar>(j);							//��i�еĵ�ַ
		uchar* data_out = result.ptr<uchar>(j);

		//������ش���
		for (int i = 0; i < numElement; ++i)
		{
			data_out[i] = data_in[i] / div * div + div / 2;                //��������
			//data[i] = data[i] - data[i]%div + div/2;                     //ȡģ
		}
	}

}
//-----------------------------------------------------------//

//---------------------------������ɨ��ͼ��------------------//
void colorReduceIter(cv::Mat image, int div = 8)
{
	//cv::MatIterator_<cv::Vec3b> it;
	//cv::Mat_ <cv::Vec3b>::iterator it;
   	//cv::MatConstIterator_<cv::Vec3b> it;
	//cv::Mat_<cv::Vec3b>::const_iterator it;                               //����������	
	auto it = image.begin<cv::Vec3b>();                                     //�ڳ�ʼλ�û�õ�����
	auto itend = image.end<cv::Vec3b>();

	for (; it != itend; ++it)
	{
		(*it)[0] = (*it)[0] / div * div + div / 2;
		(*it)[1] = (*it)[1] / div * div + div / 2; 
		(*it)[2] = (*it)[2] / div * div + div / 2;
	}
}
//-----------------------------------------------------------//

//--------------------Laplace��ͼ��------------------------//
void Sharpen(const cv::Mat& image,cv::Mat& result)
{
	result.create(image.size(), image.type());                                //����ͼ������,�����Ҫ�����
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
				- next_row[i]-pre_row[i]);                                     //�ѽ��������0~255��,8λ�޷�������
		}
	}
	//δ������������Ϊ0
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

//------------------ͼ����ӳ��-------------------------------//
void wave(const cv::Mat& image, cv::Mat& result)
{	
	//ӳ�����    srcX������ӳ�����е�����     srcY������ӳ�����е�����
	cv::Mat srcX(image.rows, image.cols,CV_32F);
	cv::Mat srcY(image.rows, image.cols, CV_32F);

	for (int y = 0; y < image.rows;++y)
	{
		for (int x = 0; x < image.cols; ++x)
		{
			srcX.at<float>(y, x) = x;
			srcY.at<float>(y, x) = y + 3*sin(x/2.0);                //ԭ��y�е�����,����һ��sin�����ƶ�
		}
	}
	cv::remap(image, result, srcX, srcY, cv::INTER_LINEAR);   //ӳ�����
}
//-----------------------------------------------------------//

int Unit2()
{
	cv::Mat image;
	image = cv::imread("back.jpg");        
	if (image.empty())
	{
		//������
		std::cout << "EMPTY" << std::endl;
		return 0;
	}
	/*
	salt(image, 2000);										    //ָ��ͬһ���ݿ�
	cv::namedWindow("salt");
	cv::imshow("salt",image);     
	*/

	/*
	colorReduceIter(image, 64);										
	cv::namedWindow("colorReduce");
	cv::imshow("colorReduce", image);
	*/

//------------------ͼ������---------------------------------//
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

//------------------�ָ�ͼ��ͨ��-----------------------------//
	/*
	cv::Mat result;
	cv::Mat imageA = cv::imread("C:\\Users\\character\\Desktop\\B.jpg");
	vector<cv::Mat> planes;                                                        //��������ͼ�������
	cv::split(imageA, planes);                                                      //�ָ�һ����ͨ����ͼ��
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

