#include "commander.h"
#include "mouseDriver.h"
#include "exceptions.h"


// this constant indicates how long after it starts for checking
#define DURATION_OF_CHECK 3000


// find opponent : check Readme for further detail

void Commander::opponentFinder() {
	try {
		contours.clear();
		cv::threshold(img, temp, 241, 255, cv::THRESH_BINARY);
		cv::erode(temp, temp, kernel1);
		cv::dilate(temp, temp, kernel2);
		cv::medianBlur(temp, temp, 5);
		cv::findContours(temp, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		if (contours.size() == 0)
			throw CHESSBOARD_DETECTION_EXCEPTION();

		momentSS.resize(1);
		momentSS[0] = cv::moments(contours[0]);
	}
	catch (CHESSBOARD_DETECTION_EXCEPTION exp) {
		std::cout << exp.get_error_message();
		getc(stdin);
		exit(0);
	}


	if ((img.size().height / 2) < float(momentSS[0].m01 / momentSS[0].m00)) {
		colorOfOpponent = black;
		std::cout << "Opponent is black !" << std::endl;
	}
	else {
		colorOfOpponent = white;
		std::cout << "Opponent is white !" << std::endl;
	}

}

// estimate size of chessboard square  (chessboard : 8x8 squareboard)

inline void Commander::calculateSquare(PointPackage &tmp) {
	chessBoardSquare = abs(tmp.lr.y - tmp.ul.y) / 8;
}


// start window recording here
Commander::Commander() {

	if (mouseDriver.catchPoints())
		windowSize = mouseDriver.getPoints();

	record(windowSize);
	record.start();
	record.getRecord(img);

}



void Commander::init() {

	opponentFinder();
	record(windowSize);
	calculateSquare(windowSize);
	econnector.connect();

}



// if opponent is black then we should start first otherwise wait for opponent response

void Commander::start() {

	if (colorOfOpponent == black) {
		std::string engineResult = econnector.get_result("");
		move += engineResult + " ";
		std::cout << "The best move : " << engineResult << std::endl;
		changePositionTable(engineResult);
		mouseDriver.execute(makeMove(engineResult));
	}

	while (!GetAsyncKeyState(VK_ESCAPE)) {
		record.start();
		record.getRecord(img);
		detectChange();

		if (change) {  //    change this line

			std::string tmp = econnector.get_result(move);
			std::cout << "The best move : " << tmp << std::endl;
			move += tmp + " ";
			changePositionTable(tmp);
			mouseDriver.execute(makeMove(tmp));
		}
		Sleep(DURATION_OF_CHECK);
		change = false;
	}
	std::cout << "Interrupt detected !" << std::endl;
	exit(0);


}



// process and get opponent position from image
// Change new table 

void Commander::imageProcessor() {

	if (colorOfOpponent == black)
		cv::threshold(img, img, 0, 255, cv::THRESH_BINARY_INV);
	else
		cv::threshold(img, img, 241, 255, cv::THRESH_BINARY);


	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (prvChessBoard[x][y] == 2)
				newChessBoard[x][y] = 2;
		}
	}

	contours.clear();
	momentSS.clear();
	cv::erode(img, img, kernel1);
	cv::dilate(img, img, kernel2);
	cv::medianBlur(img, img, 5);
	cv::findContours(img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	momentSS.resize(contours.size());

	for (int x = 0; x < contours.size(); x++) {
		momentSS[x] = cv::moments(contours[x]);
		int a = floor(float(momentSS[x].m01 / momentSS[x].m00) / chessBoardSquare);
		int b = floor(float(momentSS[x].m10 / momentSS[x].m00) / chessBoardSquare);
		newChessBoard[a][b] = 1;
	}

}


void Commander::detectChange() {

	this->imageProcessor();

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (newChessBoard[x][y] != prvChessBoard[x][y]) {
				if ((prvChessBoard[x][y] == 1 && newChessBoard[x][y] == 0)) {
					mouseMove.ul.x = x;
					mouseMove.ul.y = y;
					change = true;
				}

				else if ((prvChessBoard[x][y] == 2 && newChessBoard[x][y] == 1)) {
					mouseMove.lr.x = x;
					mouseMove.lr.y = y;
					change = true;
				}

				else if ((prvChessBoard[x][y] == 0 && newChessBoard[x][y] == 1)) {
					mouseMove.lr.x = x;
					mouseMove.lr.y = y;
					change = true;

				}



			}
		}
	}


	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			prvChessBoard[x][y] = newChessBoard[x][y];
			if (newChessBoard[x][y] != 2 || newChessBoard[x][y] != 1)
				newChessBoard[x][y] = 0;
		}
	}

	if (change) {

		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++)
				std::cout << prvChessBoard[x][y] << " ";

			std::cout << std::endl;
		}
		std::cout << std::endl;
		prepareMove();
	}


}

// find column from given coordinates like : e2e4

int Commander::findColumn(const char &x) {
	for (int z = 0; z < 8; z++) {
		if (headers[z] == x) {
			if (colorOfOpponent == black)
				return z;
			else
				return (7 - z);
		}
	}
	return 0;
}




// refresh table positions after the change

void Commander::changePositionTable(std::string move) {
	char str_char[4];
	strncpy(str_char, move.c_str(), 4);

	if (colorOfOpponent == white) {
		prvChessBoard[(str_char[1] - '0' - 1)][findColumn(str_char[0])] = 0;
		prvChessBoard[(str_char[3] - '0' - 1)][findColumn(str_char[2])] = 2;
	}
	else {
		prvChessBoard[8 - int(str_char[1] - '0')][findColumn(str_char[0])] = 0;
		prvChessBoard[8 - int(str_char[3] - '0')][findColumn(str_char[2])] = 2;

	}

}



// prepare move string representation

void Commander::prepareMove() {

	if (colorOfOpponent == white) {
		move += (headers[7 - mouseMove.ul.y] + std::to_string(mouseMove.ul.x + 1));
		move += (headers[7 - mouseMove.lr.y] + std::to_string(mouseMove.lr.x + 1) + " ");
	}
	else {
		move += (headers[mouseMove.ul.y] + std::to_string(8 - mouseMove.ul.x));
		move += (headers[mouseMove.lr.y] + std::to_string(8 - mouseMove.lr.x) + " ");
	}

	std::cout << move << std::endl;

}


// deconstract everthing

Commander::~Commander() {
	img.release();
	temp.release();
	econnector.close();
}


// click middle point of moment 
// we assume middle point of moment is exact middle point of chessboard piece

PointPackage Commander::makeMove(std::string move) {
	PointPackage ret;
	char arr[4];
	strncpy(arr, move.c_str(), 4);


	if (colorOfOpponent == white) {
		ret.ul.x = windowSize.ul.x + chessBoardSquare * (findColumn(arr[0])) + chessBoardSquare / 2;
		ret.ul.y = windowSize.ul.y + chessBoardSquare * (arr[1] - '0' - 1) + chessBoardSquare / 2;
		ret.lr.x = windowSize.ul.x + chessBoardSquare * (findColumn(arr[2])) + chessBoardSquare / 2;
		ret.lr.y = windowSize.ul.y + chessBoardSquare * (arr[3] - '0' - 1) + chessBoardSquare / 2;
	}
	else {

		ret.ul.x = windowSize.ul.x + chessBoardSquare * (findColumn(arr[0])) + chessBoardSquare;
		ret.ul.y = windowSize.ul.y + chessBoardSquare * (8 - (arr[1] - '0')) + chessBoardSquare / 2;
		ret.lr.x = windowSize.ul.x + chessBoardSquare * (findColumn(arr[2])) + chessBoardSquare;
		ret.lr.y = windowSize.ul.y + chessBoardSquare * (8 - (arr[3] - '0')) + chessBoardSquare / 2;
	}

	return ret;
}
