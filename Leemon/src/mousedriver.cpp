
#include "mousedriver.h"
#include <Windows.h>
#include <iostream>

// Wait SLEEP_TIME between each mouse click -> between 50 and 200 is better -> is best 65

#define CLICK_SLEEP_TIME 65

// Wait between each mouse movement higher is better

#define MOVE_SLEEP_TIME 330


// move cursor left right up and down 
void moveCursor(POINT point) {
	SetCursorPos(point.x, point.y);
	Sleep(CLICK_SLEEP_TIME);
	INPUT inp = { 0 };
	inp.type = INPUT_MOUSE;
	inp.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &inp, sizeof(inp));
	Sleep(CLICK_SLEEP_TIME);
	ZeroMemory(&inp, sizeof(inp));
	inp.type = INPUT_MOUSE;
	inp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &inp, sizeof(inp));
	Sleep(CLICK_SLEEP_TIME);
}


// catch points user wants to indicate if there is problem repeat that section  

bool MouseDriver::catchPoints() {
	int option;
	std::cout << "To recognize chess board correctly software needs 2 points " << std::endl;
	std::cout << "( just move the mouse to location and click left mouse button )" << std::endl;


section1:
	std::cout << std::endl;
	std::cout << " - Specify first (LEFT_TOP) point " << std::endl;
	while (!(GetKeyState(VK_LBUTTON) & 0x100));
	GetCursorPos(&ul);
	std::cout << "Mouse Position captured !   (" << ul.x << "," << ul.y << ")" << std::endl;

section2:
	std::cout << "The mouse position for first (LEFT_TOP) point is correct ?  (y/n) >>>";
	option = std::getchar();
	if (option == 'y' || option == 'Y') {
		std::getchar();
		goto section3;
	}
	else if (option == 'N' || option == 'n')
	{
		std::getchar();
		goto section1;
	}
	else {
		std::getchar();
		std::cout << "Please just use y/n or Y/N  letters !" << std::endl;
		goto section2;
	}
section3:
	std::cout << std::endl;
	std::cout << " - Specify second (RIGHT_BOTTOM) point " << std::endl;
	while (!(GetKeyState(VK_LBUTTON) & 0x100));
	GetCursorPos(&lr);
	std::cout << "Mouse Position captured !   (" << lr.x << "," << lr.y << ")" << std::endl;

section4:
	std::cout << "The mouse position for second (RIGHT_BOTTOM) point is correct ?  (y/n) >>>";
	option = std::getc(stdin);
	if (option == 'y' || option == 'Y')
		std::getchar();

	else if (option == 'N' || option == 'n')
	{
		std::getchar();
		goto section3;
	}

	else {
		std::getchar();
		std::cout << "Please just use y/n or Y/N  letters !" << std::endl;
		goto section4;
	}



	std::cout << "Operation succesfull " << std::endl;
	return true;

}


PointPackage MouseDriver::getPoints() const {
	PointPackage tmp;
	tmp.ul = ul;
	tmp.lr = lr;
	return tmp;
}

void MouseDriver::execute(PointPackage tmp) {

	moveCursor(tmp.ul);
	Sleep(MOVE_SLEEP_TIME);
	moveCursor(tmp.lr);
	Sleep(MOVE_SLEEP_TIME);
	SetCursorPos(0, 0);
	Sleep(CLICK_SLEEP_TIME);
}


