#pragma once


#include <Windows.h>
#include "pointpackage.h"

class MouseDriver {

	// points for chessboard detection upper_left and lower_right 

private: POINT ul, lr; //first ,second

public:
	PointPackage getPoints() const;
	void execute(PointPackage);
	bool catchPoints();
};


