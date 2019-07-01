#include "unit.h"
#include "Unit5.h"
#include "WaterShedSegmenter.h"
#include "MSERFeatures.h"

MorphoFeatures::MorphoFeatures() :threshold(-1), cross(5, 5), diamond(5, 5), square(5, 5), x(5, 5)
{
	cross << 0, 0, 1, 0, 0,
			0, 0, 1, 0, 0,
			1, 1, 1, 1, 1,
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0;

	diamond << 0, 0, 1, 0, 0,
			   0, 1, 1, 1, 0,
			   1, 1, 1, 1, 1,
			   0, 1, 1, 1, 0,
			   0, 0, 1, 0, 0;

	square << 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1,
			1, 1, 1, 1, 1,
			1, 1, 1, 1, 1,
			1, 1, 1, 1, 1;

	x << 1, 0, 0, 0, 1,
		0, 1, 0, 1, 0,
		0, 0, 1, 0, 0,
		0, 1, 0, 1, 0,
		1, 0, 0, 0, 1;

}


MorphoFeatures::~MorphoFeatures()
{
}

cv::Mat MorphoFeatures::getConers(const cv::Mat& image)
{
	cv::Mat result;
	cv::dilate(image, result, cross);
	cv::erode(result, result, diamond);
	cv::imshow("result", result);

	cv::Mat result2;
	cv::dilate(image, result2, x);
	cv::erode(result2, result2, square);
	cv::imshow("result2", result2);

	cv::absdiff(result2, result, result);
	cv::imshow("min", result);

	this->appltThreshold(result, result);

	return result;
}

cv::Mat MorphoFeatures::appltThreshold(const cv::Mat& image, const cv::Mat& outputImage)
{
	if (threshold <= 0.0)
		return image;
	cv::threshold(image, outputImage, threshold, 255, CV_THRESH_BINARY);
	return outputImage;

}

vector<cv::Point> MorphoFeatures::returnConers(const cv::Mat& image)
{
	vector<cv::Point> Points;
	cv::Point point;
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			if (static_cast<int>(image.at<uchar>(j,i)) > threshold)
				Points.push_back(cv::Point(j, i));
		}
	}
	return Points;
}

void MorphoFeatures::drawPoints(const cv::Mat& image,vector<cv::Point> Points)
{
	for (auto iter = Points.begin(); iter != Points.end(); ++iter)
	{
		cv::circle(image, cv::Point(iter->x,iter->y), 3, cv::Scalar(255));
	}

}
int Unit5()
{
	
	cv::Mat image = cv::imread("view.jpg", CV_LOAD_IMAGE_COLOR);
	//resize(image,image,cv::Size(image.rows / 2, image.cols / 2));
	//image = 255 - image;							//»°∑¥
	if (image.empty())
		return - 1;

	cv::Mat element(3, 3, CV_8U, cv::Scalar(1));
/*	
	cv::Mat eroded;
	//cv::erode(image, eroded, cv::Mat());							//erode
	cv::erode(image, eroded, element);								//erode

	cv::Mat dilated;
	cv::dilate(image, dilated, cv::Mat());							//dilate

	cv::Mat closed;
	cv::morphologyEx(image, closed, cv::MORPH_CLOSE, element);		//close
	cv::namedWindow("close");
	cv::imshow("close", eroded);

	cv::Mat opened;
	cv::morphologyEx(image, opened, cv::MORPH_OPEN, element);		//open
	cv::namedWindow("open");
	cv::imshow("open", dilated);
*/


//-------------------------ºÏ≤‚±ﬂ—ÿ Ω«µ„--------------//
	//cv::Mat result;
	//MorphoFeatures morpho;
	//vector<cv::Point> Points;
	//cv::morphologyEx(image, result, cv::MORPH_GRADIENT, element);							//Beuchaer:≈Ú’Õ∫Õ∏Ø ¥∫Ûµ√µΩ±ﬂ‘µ
	//morpho.setThreshold(130.0);

	//result = morpho.getConers(image);
	//Points = morpho.returnConers(result);
	//morpho.drawPoints(image, Points);
	//cv::namedWindow("Ω«µ„ºÏ≤‚");
	//cv::imshow("Ω«µ„ºÏ≤‚", image);
//----------------------------------------------------//

//--------------------WaterShed Segment---------------//
	//cv::Mat mask(image.size(),CV_8U,cv::Scalar(0));
	//cv::rectangle(mask, cv::Point(5, 5), cv::Point(image.cols - 5, image.rows - 5), cv::Scalar(255), 3);
	//cv::rectangle(mask, cv::Point((image.cols / 2) - 10, (image.rows / 2) - 10),
	//	cv::Point((image.cols / 2) + 10, (image.rows / 2) + 10), cv::Scalar(1), 10);

	//WaterShedSegmenter segmenter;
	//segmenter.setMarker(mask);
	//cv::Mat result;
	//segmenter.process(image);
	//result = segmenter.getSegmentation();
	//cv::imshow("result", result);
//----------------------------------------------------//

//--------------------MSER----------------------------//
	//cv::Mat result;
	//MSERFeatures mser(200,10000,7);
	//vector<vector<cv::Point>> rects;
	//mser.getBoundingRects(image, rects);
	//result = mser.drawRectsOnImage(image, rects);
	//cv::imshow("result", result);
//----------------------------------------------------//

//--------------------Grab Cut------------------------//
	cv::Mat result;
	cv::Rect rectangle(0, 35, 375, 215);
	cv::Mat bgmodel, fgmodel;
	cv::grabCut(image, result, rectangle, bgmodel, fgmodel, 5, cv::GC_INIT_WITH_RECT);
	cv::compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);
	//result = result & 1;
	cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255,255,255));
	image.copyTo(foreground, result);
	cv::imshow("result", foreground);
//----------------------------------------------------//
	return 0;
}