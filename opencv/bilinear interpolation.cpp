#include "Unit.h"


cv::Mat BilinearInterpolation(cv::Mat InPutImage,double scale_X,double scale_Y)
{
	if (InPutImage.channels() == 3)
	{
		cv::Mat OutputImage(static_cast<int>(scale_Y) * InPutImage.rows,
							static_cast<int>(scale_X) * InPutImage.cols,CV_8UC3);

	}
	else if (InPutImage.channels() == 1)
	{
		cv::Mat OutputImage(static_cast<int>(scale_Y * InPutImage.rows),
							static_cast<int>(scale_X * InPutImage.cols), CV_8UC1);

		int    numElement = OutputImage.cols * OutputImage.channels();

		for (int i = 0; i < static_cast<int>(scale_Y * InPutImage.rows); ++i)
		{
			uchar* data = OutputImage.ptr<uchar>(i);
			for (int j = 0; j < numElement; ++j)
			{
				double Y = i / scale_Y ;             //Rows
				double X = j / scale_X ;             //Cols
				double prePix_X = X;
				double nextPix_X = prePix_X + 1;
				double prePix_Y = Y;
				double nextPix_Y = prePix_Y + 1;

				if (nextPix_Y >= InPutImage.rows || nextPix_X >= InPutImage.cols)
					continue;
				data[j] = InPutImage.at<uchar>(static_cast<int>(nextPix_Y), static_cast<int>(prePix_X)) * (nextPix_X - X) * (nextPix_Y - Y)
					    + InPutImage.at<uchar>(static_cast<int>(nextPix_Y), static_cast<int>(nextPix_X)) * (X - prePix_X) * (nextPix_Y - Y)
					    + InPutImage.at<uchar>(static_cast<int>(prePix_Y), static_cast<int>(prePix_X)) * (nextPix_X - X) * (Y - prePix_Y)
					    + InPutImage.at<uchar>(static_cast<int>(prePix_Y), static_cast<int>(nextPix_X)) * (X - prePix_X) * (Y - prePix_Y);
				
			}
		}
		return  OutputImage;
	}
	else
	{
		return  InPutImage;
	}



	

}
