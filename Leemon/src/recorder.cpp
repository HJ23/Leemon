#include "recorder.h"


Recorder::Recorder() {}


void Recorder::operator()(PointPackage pointpack) {
	firstPoint= pointpack.ul;
	desktop = GetDesktopWindow();
	height = abs(pointpack.lr.y - pointpack.ul.y);
	width = abs(pointpack.lr.x - pointpack.ul.x);
}


// this method uses Windows API for capturing screenshots

void Recorder::start() {

	hwindowDC = GetDC(desktop);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);
	RECT size;
	GetClientRect(desktop, &size);
	record.create(height, width, CV_8UC4);

	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bitmapInfo.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.biWidth = width;
	bitmapInfo.biHeight = -height;  //this is the line that makes it draw upside down or not
	bitmapInfo.biPlanes = 1;
	bitmapInfo.biBitCount = 32;
	bitmapInfo.biCompression = BI_RGB;
	bitmapInfo.biSizeImage = 0;
	bitmapInfo.biXPelsPerMeter = 0;
	bitmapInfo.biYPelsPerMeter = 0;
	bitmapInfo.biClrUsed = 0;
	bitmapInfo.biClrImportant = 0;

	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, firstPoint.x, firstPoint.y, width, height, SRCCOPY);
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, record.data, (BITMAPINFO *)&bitmapInfo, DIB_RGB_COLORS);

	DeleteObject(hbwindow); DeleteDC(hwindowCompatibleDC); ReleaseDC(desktop, hwindowDC);
	cvtColor(record, record, cv::COLOR_BGR2GRAY);
}

// for debug purposes 
void Recorder::save() {
	imwrite(".", record);  //specify path for screenshot
}


void Recorder::getRecord(cv::Mat &img) {
	record.copyTo(img);
}


Recorder::~Recorder() {
	record.release();
}
