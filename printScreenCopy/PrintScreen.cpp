#include "stdafx.h"
#include "PrintScreen.h"
#include "GraphicsInterface\GraphicsInterface.h"
#include "resource.h"
#include <windowsx.h> // For GET_X_LPARAM and GET_X_LPARAM macros
#include <string>
#include <atlimage.h> // For Cimage class

MyPrintScreen* MyPrintScreen::thisPtr = nullptr;


MyPrintScreen::MyPrintScreen(HINSTANCE hInstanceIn)
{
	thisPtr = this;
	const int C_DEFAULT_WINDOW_2_ALPHA = 0x90;
	maskA = C_DEFAULT_WINDOW_2_ALPHA;
	hInstance = hInstanceIn;

	// Create window 1 (background window)
	int exWindowStyles = WS_EX_LAYERED;
	int windowStyles = WS_POPUP;
	int classStyles = 0;
	hwnd1 = BasicWindowMaker::create_window(L"Screenshot Tool", L"screenshotToolClass",
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) + 1,
		WndProc, hInstance, SW_SHOW, exWindowStyles, windowStyles, classStyles,
		0, IDI_PRINTSCREENCOPY, IDI_SMALL);
	gi1 = new GraphicsInterface(hwnd1);
	SetLayeredWindowAttributes(hwnd1, 0, 1, LWA_ALPHA);

	// Create window 2 (foreground)
	exWindowStyles = WS_EX_LAYERED | WS_EX_TRANSPARENT;
	windowStyles = WS_POPUP;
	classStyles = 0;
	hwnd2 = BasicWindowMaker::create_window(L"Screenshot Tool extra", L"screenshotToolClass",
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) + 1,
		WndProc, hInstance, SW_SHOW, exWindowStyles, windowStyles, classStyles,
		0, IDI_PRINTSCREENCOPY, IDI_SMALL, hwnd1); // the last argument makes window 2 a child of window 1
	gi2 = new GraphicsInterface(hwnd2);
	SetLayeredWindowAttributes(hwnd2, RGB(1, 1, 1), maskA, LWA_ALPHA | LWA_COLORKEY);
	
	// Initialize fields
	lrClickFocus = false;
	RECT rectClient;
	GetClientRect(hwnd1, &rectClient);
	rectCaptureLeft = rectClient.right / 2 - 150;
	rectCaptureRight = rectClient.right / 2 + 150;
	rectCaptureTop = rectClient.bottom / 2 - 150;
	rectCaptureBottom = rectClient.bottom / 2 + 150;
	maskR = 0;
	maskG = 0;
	maskB = 0;
	
	draw_window();
}


			// Public methods

void MyPrintScreen::size_change()
{
	gi1->size_change();
	gi2->size_change();
}

void MyPrintScreen::left_click(int xP, int yP)
{
	lrClickFocus = false;
	rectCaptureLeft = xP;
	if (xP >= rectCaptureRight)
		rectCaptureRight = xP + 50;

	rectCaptureTop = yP;
	if (yP >= rectCaptureBottom)
		rectCaptureBottom = yP + 50;

	draw_window();
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

	draw_window();
}

void MyPrintScreen::middle_click()
{
	static int colorMode = 0;
	colorMode++;
	if (colorMode > 3)
		colorMode = 0;

	if (colorMode == 0)
	{
		maskR = 0;
		maskG = 0;
		maskB = 0;
	}
	else if (colorMode == 1)
	{
		maskR = 0xFF;
		maskG = 0xFF;
		maskB = 0xFF;
	}
	else if (colorMode == 2)
	{
		maskR = 0x00;
		maskG = 0xE0;
		maskB = 0xF0;
	}
	else if (colorMode == 3)
	{
		maskR = 0xFF;
		maskG = 0x00;
		maskB = 0x00;
	}

	draw_window();
}

void MyPrintScreen::mouse_wheel(int amount)
{
	// Calculate the window 2's new alpha
	maskA += amount * 0x10;
	if (maskA > 0xFF)
		maskA = 0xFF;
	else if (maskA < 0)
		maskA = 0;

	// Set the window's alpha
	SetLayeredWindowAttributes(hwnd2, RGB(1, 1, 1), maskA, LWA_ALPHA | LWA_COLORKEY);
}

void MyPrintScreen::up_arrow()
{
	if (!lrClickFocus)
		set_rct_offset_smart(-1);
	else
		set_rcb_offset_smart(-1);

	draw_window();
}

void MyPrintScreen::down_arrow()
{
	if (!lrClickFocus)
		set_rct_offset_smart(1);
	else
		set_rcb_offset_smart(1);

	draw_window();
}

void MyPrintScreen::left_arrow()
{
	if (!lrClickFocus)
		set_rcl_offset_smart(-1);
	else
		set_rcr_offset_smart(-1);

	draw_window();
}

void MyPrintScreen::right_arrow()
{
	if (!lrClickFocus)
		set_rcl_offset_smart(1);
	else
		set_rcr_offset_smart(1);

	draw_window();
}

void MyPrintScreen::draw_window()
{
	// Draw window 1
	RECT rectClient;
	GetClientRect(hwnd1, &rectClient);
	gi1->draw_rect(0, 0, rectClient.right, rectClient.bottom, 0xFF, 0xFF, 0xFF);
	gi1->copy_buffer();

	// Draw window 2
	GetClientRect(hwnd2, &rectClient);
	gi2->draw_rect(0, 0, rectClient.right, rectClient.bottom, maskR, maskG, maskB);
	gi2->draw_rect(rectCaptureLeft, rectCaptureTop, get_width(), get_height(), 1, 1, 1);
	gi2->copy_buffer();
}

void MyPrintScreen::pressed_g()
{
	screenshot_region_and_save(L".gif", L"Saved picture as '.gif' in 'pics' folder.");
}

void MyPrintScreen::pressed_j()
{
	screenshot_region_and_save(L".jpg", L"Saved picture as '.jpg' in 'pics' folder.");
}

void MyPrintScreen::pressed_p()
{
	screenshot_region_and_save(L".png", L"Saved picture as '.png' in 'pics' folder.");
}

void MyPrintScreen::pressed_b()
{
	screenshot_region_and_save(L".bmp", L"Saved picture as '.bmp' in 'pics' folder.");
}


			// Private methods

void MyPrintScreen::set_rcl_offset_smart(int xP)
{
	if (rectCaptureLeft + xP < rectCaptureRight)
		rectCaptureLeft += xP;
}

void MyPrintScreen::set_rcr_offset_smart(int xP)
{
	if (rectCaptureRight + xP > rectCaptureLeft)
		rectCaptureRight += xP;
}

void MyPrintScreen::set_rct_offset_smart(int yP)
{
	if (rectCaptureTop + yP < rectCaptureBottom)
		rectCaptureTop += yP;
}

void MyPrintScreen::set_rcb_offset_smart(int yP)
{
	if (rectCaptureBottom + yP > rectCaptureTop)
		rectCaptureBottom += yP;
}

int MyPrintScreen::get_width()
{
	return rectCaptureRight - rectCaptureLeft;
}

int MyPrintScreen::get_height()
{
	return rectCaptureBottom - rectCaptureTop;
}

void save_rect_on_screen_as_image(int x, int y, int w, int h, std::wstring fileName)
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

void MyPrintScreen::screenshot_region_and_save(LPCWSTR fileExtension, LPCWSTR mbMessage)
{
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

	ShowWindow(hwnd1, SW_HIDE);
	ShowWindow(hwnd2, SW_HIDE);
	save_rect_on_screen_as_image(rectCaptureLeft, rectCaptureTop, get_width(), get_height(), fileName);
	ShowWindow(hwnd1, SW_SHOW);
	ShowWindow(hwnd2, SW_SHOW);
	draw_window();
	MessageBox(0, mbMessage, L"Screen Captured", MB_OK);
}


LRESULT CALLBACK MyPrintScreen::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_ERASEBKGND:
		break;
	case WM_KEYDOWN:
		{
			if (wParam == VK_DOWN)
				thisPtr->down_arrow();
			else if (wParam == VK_UP)
				thisPtr->up_arrow();
			else if (wParam == VK_RIGHT)
				thisPtr->right_arrow();
			else if (wParam == VK_LEFT)
				thisPtr->left_arrow();
			else if (wParam == 'B')
				thisPtr->pressed_b();
			else if (wParam == 'G')
				thisPtr->pressed_g();
			else if (wParam == 'J')
				thisPtr->pressed_j();
			else if (wParam == 'P')
				thisPtr->pressed_p();
			else if (wParam == VK_ESCAPE)
				PostQuitMessage(0);
		}
		break;
	case WM_LBUTTONDOWN:
		thisPtr->left_click(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_RBUTTONDOWN:
		thisPtr->right_click(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MBUTTONDOWN:
		thisPtr->middle_click();
		break;
	case WM_MOUSEWHEEL:
		thisPtr->mouse_wheel(GET_WHEEL_DELTA_WPARAM(wParam) / 120);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
