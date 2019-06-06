#pragma once
#include <core.hpp>
#include <highgui.hpp>
#include <imgproc.hpp>
#include <iostream>

using namespace std;

int Unit1();
int Unit2();
int Unit3();
cv::Mat BilinearInterpolation(cv::Mat InPutImage,double scale_X, double scale_Y);
int GradientDescent();