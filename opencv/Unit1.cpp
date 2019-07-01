#include <../opencv2/core/core.hpp>
#include <../opencv2/highgui/highgui.hpp>
#include <../opencv2/imgproc.hpp>
#include <iostream>
using namespace std;

//�ص�����

void onMouse(int event, int x, int y, int flags, void* param)       
{
	cv::Mat* im = reinterpret_cast<cv::Mat*>(param);
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		std::cout << "at (" << x << "," << y << ") value is:" << 
				static_cast<int>(im->at<uchar>(cv::Point(x, y))) << std::endl;          //��ȡ���λ���Լ�����ֵ
		break;
	}
}

int Unit1()
{
	cv::Mat image;
	cv::Mat result;
	image = cv::imread("back.jpg",CV_LOAD_IMAGE_COLOR);				//CV_LOAD_IMAGE_GRAYSCALE
	cv::resize(image,image,cv::Size(1400,760));                     //zoom
	std::cout << "Channels:" << image.channels() << endl;

	if (image.empty())
	{
		//������
		std::cout << "EMPTY" << std::endl;
		return 0;
	}

	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);     

//-------------cv::flip-------------------------------//
	/*
	cv::namedWindow("Flip Image");									//��תͼ��
	cv::flip(image,result,0);										// >0: horizontal  0��vertical  <0:horizonal and vertical
	cv::imshow("Flip Image", result);
   */
//----------------------------------------------------//

//-------------Draw-----------------------------------//
	/*
	cv::namedWindow("Drawaing Image");
	cv::circle(image,cv::Point(155,110),75,0,3);                  //circle ratangle ellipse line
	cv::putText(image,"sop2",cv::Point(40,200),cv::FONT_HERSHEY_PLAIN,2.0,cv::Scalar(0,0,128),2);          //д���ı�
	cv::imshow("Drawaing Image", image);
	*/
//----------------------------------------------------//

//-------------callback function----------------------//
	
	cv::setMouseCallback("Original Image", onMouse, reinterpret_cast<void*>(&image));         //ע��ص�������ͼ���ַ��Ϊ����																			        //�������͸�����
	
//----------------------------------------------------//
	
//-------------cv::Mat--------------------------------//
	/*
	cv::namedWindow("Iamge1");
	cv::Mat image1(240,400,CV_8UC3,cv::Scalar(0,0,255));			//ͨ������BGR
	cv::Mat image2;
	cv::Mat image3;
	cv::Mat image4;
	image1.convertTo(image3,CV_32F, 1 / 255.0, 0.0);                //���������Ͳ�һ��

	image1.copyTo(image2);                                          //����  conle
	image4 = image;                                                 //ָ��ͬһ���ݿ�
	cv::flip(image1, image1, -1);
	imshow("Iamge1",image3);
	*/
//----------------------------------------------------//

//--------------------Iplimage------------------------//
	/*
	cv::Ptr<IplImage> iplImage = cvLoadImage("sop.jpg");          //C���Խӿ�װ�غ���    
	IplImage* iplImage = cvLoadImage("sop.jpg");                  //��Ҫ�ͷ�iplImage�ṹָ����ڴ�
	cv::Mat iamge(iplImage);                                      //ת��Ϊcv::Mat����
	cvReleaseImage(&iplImage);     
	*/                               //�ͷ�
//----------------------------------------------------//

//--------------------Roi-----------------------------//
	cv::Mat logo = cv::imread("front.jpg", CV_LOAD_IMAGE_COLOR);
	cv::Mat imageROI = image(cv::Range(image.rows - logo.rows,image.rows),
		cv::Range(image.cols - logo.cols, image.cols));				    //����һ��ROI����
	cv::Mat mask = cv::imread("front.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	logo.copyTo(imageROI,mask);											//cloneʧЧ
	cv::imshow("Original Image", image);
//----------------------------------------------------//	
	//cv::imwrite("vertical flip",result);							    //save
	cv::waitKey(0);
	return 0;
}
