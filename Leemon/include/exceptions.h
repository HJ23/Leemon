#pragma once
#include <string>


// abstract class for exceptions

class EXCEPTION {
public:
	std::string error_message;
	std::string get_error_message() { return error_message; };
	void set_error_message() {};

};


class ENGINE_ERROR_EXCEPTION : public EXCEPTION {
public:
	ENGINE_ERROR_EXCEPTION() { error_message = "Could not find any ENGINE in the path !"; };

};

class CHESSBOARD_DETECTION_EXCEPTION :public EXCEPTION {
public:
	CHESSBOARD_DETECTION_EXCEPTION() { error_message = "Please indicate chessboard UPPER-LEFT and LOWER-RIGHT coordinates properly !"; };
};
