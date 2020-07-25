/*
  This header file contains  desktop screenshot capturing and processing methods
  mostly Windows API related functions
  */


#pragma once
#include <Windows.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include "pointpackage.h"

using cv::Mat;

class Recorder {
private:

	cv::Mat record;   //image_file
	HWND desktop; //
	HDC hwindowDC, hwindowCompatibleDC;
	BITMAPINFOHEADER  bitmapInfo;  //bi
	HBITMAP hbwindow;
	int height, width;
	POINT firstPoint;

public: Recorder();
		void start();
		void save();
		void getRecord(Mat &);
		void operator()(PointPackage);
		~Recorder();
};

