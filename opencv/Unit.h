#pragma once
#include <core.hpp>
#include <highgui.hpp>
#include <imgproc.hpp>
#include <iostream>
#include <features2d.hpp>

using namespace std;

int Unit1();
int Unit2();
int Unit3();
int Unit4();
int Unit5();
int Unit6();
int Unit7();
int Unit8();
cv::Mat BilinearInterpolation(cv::Mat InPutImage,double scale_X, double scale_Y);
int GradientDescent();