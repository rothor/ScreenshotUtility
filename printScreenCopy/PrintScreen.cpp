#include "stdafx.h"
#include "PrintScreen.h"
#include <windows.h>
#include <stdio.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
#include <windowsx.h>
#include <string>
#include <atlstr.h>
#include <atlimage.h>
#include <Gdiplusimaging.h>


void MyPrintScreen::set_hwnd(HWND hWndP)
{
	hWnd = hWndP;

	RECT rectClient;
	GetClientRect(hWnd, &rectClient);
	rectCaptureLeft = rectClient.right / 2 - 150;
	rectCaptureRight = rectClient.right / 2 + 150;
	rectCaptureTop = rectClient.bottom / 2 - 150;
	rectCaptureBottom = rectClient.bottom / 2 + 150;
}

void MyPrintScreen::repaint_scene()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	RECT rectClient;
	GetClientRect(hWnd, &rectClient);
	RECT rectWindow;
	GetWindowRect(hWnd, &rectWindow);
	HDC backHdc = CreateCompatibleDC(hdc);
	HBITMAP backbuffer = CreateCompatibleBitmap(hdc, rectClient.right, rectClient.bottom);
	SelectObject(backHdc, backbuffer);

	//drawing starts ->
	Gdiplus::Graphics graphics(backHdc);

	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));
	graphics.FillRectangle(&blackBrush, 0, 0, rectClient.right, rectClient.bottom);

	Gdiplus::SolidBrush transBrush(Gdiplus::Color(255, 200, 200, 200));
	POINT ptTl;
	ptTl.x = rectCaptureLeft;
	ptTl.y = rectCaptureTop;
	ScreenToClient(hWnd, &ptTl);
	graphics.FillRectangle(&transBrush, ptTl.x, ptTl.y, get_width(), get_height());
	//drawing ends ->

	BitBlt(hdc, 0, 0, rectClient.right, rectClient.bottom, backHdc, 0, 0, SRCCOPY);
	DeleteObject(backbuffer);
	DeleteDC(backHdc);

	EndPaint(hWnd, &ps);
}

void MyPrintScreen::capture_image_and_save(LPCWSTR fileExtension, LPCWSTR mbMessage)
{
	ShowWindow(hWnd, SW_HIDE);
	save_square_on_screen_as_file(fileExtension);
	ShowWindow(hWnd, SW_SHOW);
	MessageBox(hWnd, mbMessage, L"Screen Captured", MB_OK);
}

void MyPrintScreen::save_square_on_screen_as_file(LPCTSTR fileExtension)
{
	HDC hdcScreen = GetDC(NULL);
	HDC hdcWindow = GetDC(hWnd);
	HDC hdcMem;
	HBITMAP hbmMem;
	BITMAP bmpScreen;

	hdcMem = CreateCompatibleDC(hdcScreen);
	hbmMem = CreateCompatibleBitmap(hdcScreen, get_width(), get_height());
	SelectObject(hdcMem, hbmMem);

	BitBlt(hdcMem, 0, 0, get_width(), get_height(), hdcScreen, rectCaptureLeft, rectCaptureTop, SRCCOPY);

	CreateDirectory(L"pics", NULL);
	SYSTEMTIME mySysTime;
	GetSystemTime(&mySysTime);
	std::wstring fileName{ L"pics/" };
	fileName += std::to_wstring(mySysTime.wMilliseconds);
	fileName += std::to_wstring(mySysTime.wSecond);
	fileName += std::to_wstring(mySysTime.wMinute);
	fileName += std::to_wstring(mySysTime.wHour);
	fileName += std::to_wstring(mySysTime.wDay);
	fileName += std::to_wstring(mySysTime.wMonth);
	fileName += std::to_wstring(mySysTime.wYear);
	fileName += fileExtension;

	CImage myCimg;
	myCimg.Attach(hbmMem);
	myCimg.Save(fileName.c_str());
}

void MyPrintScreen::redraw_window()
{
	repaint_scene();
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
}


			/* User Input */

void MyPrintScreen::left_click(int xP, int yP)
{
	lrClickFocus = false;
	rectCaptureLeft = xP;
	if (xP >= rectCaptureRight)
		rectCaptureRight = xP + 50;

	rectCaptureTop = yP;
	if (yP >= rectCaptureBottom)
		rectCaptureBottom = yP + 50;

	void redraw_window();
}

void MyPrintScreen::right_click(int xP, int yP)
{
	lrClickFocus = true;
	rectCaptureRight = xP;
	if (xP <= rectCaptureLeft)
		rectCaptureLeft = xP - 50;

	rectCaptureBottom = yP;
	if (yP <= rectCaptureTop)
		rectCaptureTop = yP - 50;

	void redraw_window();
}

void MyPrintScreen::up_arrow()
{
	if (!lrClickFocus)
		set_rct_offset_smart(-1);
	else
		set_rcb_offset_smart(-1);

	void redraw_window();
}

void MyPrintScreen::down_arrow()
{
	if (!lrClickFocus)
		set_rct_offset_smart(1);
	else
		set_rcb_offset_smart(1);

	void redraw_window();
}

void MyPrintScreen::left_arrow()
{
	if (!lrClickFocus)
		set_rcl_offset_smart(-1);
	else
		set_rcr_offset_smart(-1);

	void redraw_window();
}

void MyPrintScreen::right_arrow()
{
	if (!lrClickFocus)
		set_rcl_offset_smart(1);
	else
		set_rcr_offset_smart(1);

	void redraw_window();
}

void MyPrintScreen::pressed_g()
{
	capture_image_and_save(L".gif", L"Saved picture as '.gif' in 'pics' folder.");
}

void MyPrintScreen::pressed_j()
{
	capture_image_and_save(L".jpg", L"Saved picture as '.jpg' in 'pics' folder.");
}

void MyPrintScreen::pressed_p()
{
	capture_image_and_save(L".png", L"Saved picture as '.png' in 'pics' folder.");
}

void MyPrintScreen::pressed_b()
{
	capture_image_and_save(L".bmp", L"Saved picture as '.bmp' in 'pics' folder.");
}