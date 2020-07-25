/*

   This class contains methods which will analyse image and make moves
   
  */


#pragma once
#include <Windows.h>
#include "PointPackage.h"
#include "MouseDriver.h"
#include "Recorder.h"
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include "engine.h"
#include <string>
#include <cstdlib>

using std::vector;
using cv::Moments;
using cv::Mat;
using cv::Point;
using cv::Size;

enum color_set { black, white };    // opponent color and user color


class Commander {

private:
	cv::Mat img;
	cv::Mat temp;
	cv::Mat pureImage;
	vector<Moments> momentSS;           // moments from image (*Moment - center coordinates of blob)
	vector<vector<Point> > contours;
	color_set colorOfOpponent;
	int chessBoardSquare;               // estimated chessboard width or height -> Assumption : chessboard is squarecv::Mat pureImage;

	PointPackage windowSize;
	PointPackage mouseMove;

	bool change = false;
	bool first = false;

	const char headers[8] = { 'a','b','c','d','e','f','g','h' };

	// previous chessboard positions important for keeping track of changes on board

	int prvChessBoard[8][8] = {   
	{ 1,1,1,1,1,1,1,1 },
	{ 1,1,1,1,1,1,1,1 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 2,2,2,2,2,2,2,2 },
	{ 2,2,2,2,2,2,2,2 }
	};

	// new chessboard will add new positions to previous chessboard 
	int newChessBoard[8][8] = {    
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0 }
	};


	// For further details please check documentation 
	// Kernels for chessboard pieces detection
	// all there kernel sizes obtained after quite long experimentation
	cv::Mat kernel1 = Mat::ones(Size(5, 5), CV_8U);
	cv::Mat kernel2 = Mat::ones(Size(15, 15), CV_8U);
	cv::Mat kernel3 = Mat::ones(Size(51, 1), CV_8U);
	cv::Mat kernel4 = Mat::ones(Size(1, 51), CV_8U);

	std::string move = "";
	MouseDriver mouseDriver;
	Connector econnector;
	Recorder record;

	void imageProcessor();
	void opponentFinder();
	inline void calculateSquare(PointPackage&);
	void detectChange();
	void prepareMove();
	void changePositionTable(std::string);
	PointPackage makeMove(std::string);
	int findColumn(const char&);


public:

	void init();
	Commander();   //initialize pass window 
	void start();  //while loop here and check interrupt !
	~Commander();
};

