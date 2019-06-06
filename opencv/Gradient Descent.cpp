#include "Unit.h"
#include <iostream>
using namespace std;

vector<cv::Point> v_Point;
void onMouseAction(int event, int x, int y, int flags, void* param)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		cv::Mat* im = reinterpret_cast<cv::Mat*>(param);
		std::cout << "at (" << x << "," << y << ") value is:" <<
			static_cast<int>(im->at<uchar>(cv::Point(x, y))) << std::endl;          //获取点击位置以及像素值
		cv::circle(*im, cv::Point(x, y), 5, cv::Scalar(0), -1);
		cv::Point n_point;
		n_point.x = x;
		n_point.y = y;
		v_Point.push_back(n_point);
		cv::imshow("Input", *im);
		break;
	}
}

int GradientDescent()
{
	cv::Mat BackGround(600, 600, CV_8UC1,cv::Scalar(255));
	if (BackGround.empty())
	{
		//错误处理
		std::cout << "EMPTY" << std::endl;
		return 0;
	}
	cv::namedWindow("Input");
	cv::imshow("Input", BackGround);
	cv::setMouseCallback("Input", onMouseAction, reinterpret_cast<void*>(&BackGround));         //注册回调函数，图像地址作为附加

	if(cv::waitKey(0));
	{
		int nSample = v_Point.size();

	    /* Mat_<uchar>--------- CV_8U
		Mat<char>-----------    CV_8S
		Nat_<short>---------    CV_16S
		Mat_<ushort>--------    CV_16U
		Mat_<int>-----------    CV_32S
		Mat_<float>----------   CV_32F
		Mat_<double>--------    CV_64F
		*/
		cv::Mat_<double> theta = cv::Mat::zeros(cv::Size(1, 2), CV_64F);
		cv::Mat_<double> X = cv::Mat::ones(cv::Size(2,nSample), CV_64F);
		cv::Mat_<double> Y(cv::Size(1,nSample), CV_64F);
		cv::Mat_<double> grandient = cv::Mat::zeros(cv::Size(1, 2), CV_64F);
		int i = 0;
		double alpha = 0.01;
		for (auto iter = v_Point.begin(); iter != v_Point.end(); ++iter)
		{
			X(i,1) = static_cast<double>(iter->x)/100;
			Y(i) = static_cast<double>(iter->y)/100;
			++i;
		}
		do
		{
			cout << X << endl;
			cout << Y << endl;
			cout << grandient << endl;
			cout << theta << endl;
			grandient = X.t() * (X * theta - Y);
			theta = theta - alpha * grandient;
			
		} while (fabs(grandient(1)) > 1e-5);

		cv::line(BackGround, cvPoint(0, theta(0, 0)*100), cvPoint(600, 600 * theta(1, 0) + theta(0, 0)*100)
			, cv::Scalar(128), 3);
		cv::imshow("Input", BackGround);
	}
	return 0;
}

