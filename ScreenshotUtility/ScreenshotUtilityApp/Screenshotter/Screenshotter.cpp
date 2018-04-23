#include "Screenshotter.h"
#include <atlimage.h> // For Cimage class


void Screenshotter::takeScreenshot(int x, int y, int w, int h, std::wstring fileName)
{
	HDC hdcScreen = GetDC(NULL);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmMem = CreateCompatibleBitmap(hdcScreen, w, h);
	SelectObject(hdcMem, hbmMem);

	BitBlt(hdcMem, 0, 0, w, h, hdcScreen, x, y, SRCCOPY);

	CImage myCimg;
	myCimg.Attach(hbmMem);
	myCimg.Save(fileName.c_str());
}