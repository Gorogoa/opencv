#include <../opencv2/core/core.hpp>
#include <../opencv2/highgui/highgui.hpp>
#include <../opencv2/imgproc.hpp>
#include <iostream>
using namespace std;

//回调函数

void onMouse(int event, int x, int y, int flags, void* param)       
{
	cv::Mat* im = reinterpret_cast<cv::Mat*>(param);
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		std::cout << "at (" << x << "," << y << ") value is:" << 
				static_cast<int>(im->at<uchar>(cv::Point(x, y))) << std::endl;          //获取点击位置以及像素值
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
		//错误处理
		std::cout << "EMPTY" << std::endl;
		return 0;
	}

	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);     

//-------------cv::flip-------------------------------//
	/*
	cv::namedWindow("Flip Image");									//翻转图像
	cv::flip(image,result,0);										// >0: horizontal  0：vertical  <0:horizonal and vertical
	cv::imshow("Flip Image", result);
   */
//----------------------------------------------------//

//-------------Draw-----------------------------------//
	/*
	cv::namedWindow("Drawaing Image");
	cv::circle(image,cv::Point(155,110),75,0,3);                  //circle ratangle ellipse line
	cv::putText(image,"sop2",cv::Point(40,200),cv::FONT_HERSHEY_PLAIN,2.0,cv::Scalar(0,0,128),2);          //写入文本
	cv::imshow("Drawaing Image", image);
	*/
//----------------------------------------------------//

//-------------callback function----------------------//
	
	cv::setMouseCallback("Original Image", onMouse, reinterpret_cast<void*>(&image));         //注册回调函数，图像地址作为附加																			        //参数发送个函数
	
//----------------------------------------------------//
	
//-------------cv::Mat--------------------------------//
	/*
	cv::namedWindow("Iamge1");
	cv::Mat image1(240,400,CV_8UC3,cv::Scalar(0,0,255));			//通道次序BGR
	cv::Mat image2;
	cv::Mat image3;
	cv::Mat image4;
	image1.convertTo(image3,CV_32F, 1 / 255.0, 0.0);                //副本，类型不一致

	image1.copyTo(image2);                                          //副本  conle
	image4 = image;                                                 //指向同一数据块
	cv::flip(image1, image1, -1);
	imshow("Iamge1",image3);
	*/
//----------------------------------------------------//

//--------------------Iplimage------------------------//
	/*
	cv::Ptr<IplImage> iplImage = cvLoadImage("sop.jpg");          //C语言接口装载函数    
	IplImage* iplImage = cvLoadImage("sop.jpg");                  //需要释放iplImage结构指向的内存
	cv::Mat iamge(iplImage);                                      //转换为cv::Mat类型
	cvReleaseImage(&iplImage);     
	*/                               //释放
//----------------------------------------------------//

//--------------------Roi-----------------------------//
	cv::Mat logo = cv::imread("front.jpg", CV_LOAD_IMAGE_COLOR);
	cv::Mat imageROI = image(cv::Range(image.rows - logo.rows,image.rows),
		cv::Range(image.cols - logo.cols, image.cols));				    //定义一个ROI区域
	cv::Mat mask = cv::imread("front.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	logo.copyTo(imageROI,mask);											//clone失效
	cv::imshow("Original Image", image);
//----------------------------------------------------//	
	//cv::imwrite("vertical flip",result);							    //save
	cv::waitKey(0);
	return 0;
}
