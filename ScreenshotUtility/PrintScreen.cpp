#include "PrintScreen.h"
#include "GraphicsInterface\GraphicsInterface.h"
#include "Resource.h"
#include <windowsx.h> // For GET_X_LPARAM and GET_X_LPARAM macros
#include <string>
#include <atlimage.h> // For Cimage class

PrintScreen* PrintScreen::thisPtr = nullptr;


PrintScreen::PrintScreen(HINSTANCE hInstanceIn)
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
		0, IDI_SCREENSHOTUTILITY, IDI_SMALL);
	gi1 = new GraphicsInterface(hwnd1);
	SetLayeredWindowAttributes(hwnd1, 0, 1, LWA_ALPHA);

	// Create window 2 (foreground)
	exWindowStyles = WS_EX_LAYERED | WS_EX_TRANSPARENT;
	windowStyles = WS_POPUP;
	classStyles = 0;
	hwnd2 = BasicWindowMaker::create_window(L"Screenshot Tool extra", L"screenshotToolClass",
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) + 1,
		WndProc, hInstance, SW_SHOW, exWindowStyles, windowStyles, classStyles,
		0, IDI_SCREENSHOTUTILITY, IDI_SMALL, hwnd1); // the last argument makes window 2 a child of window 1
	gi2 = new GraphicsInterface(hwnd2);
	SetLayeredWindowAttributes(hwnd2, RGB(1, 1, 1), maskA, LWA_ALPHA | LWA_COLORKEY);
	
	// Initialize fields
	mouseRightButtonLastClicked = false;
	RECT rectClient;
	GetClientRect(hwnd1, &rectClient);
	rectCapture.left = rectClient.right / 2 - 150;
	rectCapture.right = rectClient.right / 2 + 150;
	rectCapture.top = rectClient.bottom / 2 - 150;
	rectCapture.bottom = rectClient.bottom / 2 + 150;
	maskR = 0;
	maskG = 0;
	maskB = 0;
	maskColorMode = 0;
	
	draw_window();
}


			// Public methods

void PrintScreen::size_change()
{
	gi1->size_change();
	gi2->size_change();
}

void PrintScreen::left_click(int xIn, int yIn)
{
	mouseRightButtonLastClicked = false;
	rectCapture.left = xIn;
	if (xIn >= rectCapture.right)
		rectCapture.right = xIn + 50;

	rectCapture.top = yIn;
	if (yIn >= rectCapture.bottom)
		rectCapture.bottom = yIn + 50;

	draw_window();
}

void PrintScreen::right_click(int xIn, int yIn)
{
	mouseRightButtonLastClicked = true;
	rectCapture.right = xIn;
	if (xIn <= rectCapture.left)
		rectCapture.left = xIn - 50;

	rectCapture.bottom = yIn;
	if (yIn <= rectCapture.top)
		rectCapture.top = yIn - 50;

	draw_window();
}

void PrintScreen::middle_click()
{
	maskColorMode++;
	if (maskColorMode > 3)
		maskColorMode = 0;

	if (maskColorMode == 0)
	{
		maskR = 0;
		maskG = 0;
		maskB = 0;
	}
	else if (maskColorMode == 1)
	{
		maskR = 0xFF;
		maskG = 0xFF;
		maskB = 0xFF;
	}
	else if (maskColorMode == 2)
	{
		maskR = 0x00;
		maskG = 0xE0;
		maskB = 0xF0;
	}
	else if (maskColorMode == 3)
	{
		maskR = 0xFF;
		maskG = 0x00;
		maskB = 0x00;
	}

	draw_window();
}

void PrintScreen::mouse_wheel(int amount)
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

void PrintScreen::key_pressed(int keyCode)
{
	if (keyCode == VK_DOWN)
	{
		if (!mouseRightButtonLastClicked)
			displace_rectCapture_top_smart(1);
		else
			displace_rectCapture_bottom_smart(1);

		draw_window();
	}
	else if (keyCode == VK_UP)
	{
		if (!mouseRightButtonLastClicked)
			displace_rectCapture_top_smart(-1);
		else
			displace_rectCapture_bottom_smart(-1);

		draw_window();
	}
	else if (keyCode == VK_RIGHT)
	{
		if (!mouseRightButtonLastClicked)
			displace_rectCapture_left_smart(1);
		else
			displace_rectCapture_right_smart(1);

		draw_window();
	}
	else if (keyCode == VK_LEFT)
	{
		if (!mouseRightButtonLastClicked)
			displace_rectCapture_left_smart(-1);
		else
			displace_rectCapture_right_smart(-1);

		draw_window();
	}
	else if (keyCode == 'B')
		screenshot_region_and_save(L".bmp", L"Saved picture as '.bmp' in 'pics' folder.");
	else if (keyCode == 'G')
		screenshot_region_and_save(L".gif", L"Saved picture as '.gif' in 'pics' folder.");
	else if (keyCode == 'J')
		screenshot_region_and_save(L".jpg", L"Saved picture as '.jpg' in 'pics' folder.");
	else if (keyCode == 'P')
		screenshot_region_and_save(L".png", L"Saved picture as '.png' in 'pics' folder.");
	else if (keyCode == VK_ESCAPE)
		PostQuitMessage(0);
}

void PrintScreen::draw_window()
{
	// Draw window 1
	RECT rectClient;
	GetClientRect(hwnd1, &rectClient);
	gi1->draw_rect(0, 0, rectClient.right, rectClient.bottom, 0xFF, 0xFF, 0xFF);
	gi1->copy_buffer();

	// Draw window 2
	GetClientRect(hwnd2, &rectClient);
	gi2->draw_rect(0, 0, rectClient.right, rectClient.bottom, maskR, maskG, maskB);
	gi2->draw_rect(rectCapture.left, rectCapture.top,
		rectCapture.right - rectCapture.left, rectCapture.bottom - rectCapture.top, 1, 1, 1);
	gi2->copy_buffer();
}


			// Private methods

void PrintScreen::displace_rectCapture_left_smart(int amount)
{
	if (rectCapture.left + amount < rectCapture.right)
		rectCapture.left += amount;
}

void PrintScreen::displace_rectCapture_right_smart(int amount)
{
	if (rectCapture.right + amount > rectCapture.left)
		rectCapture.right += amount;
}

void PrintScreen::displace_rectCapture_top_smart(int amount)
{
	if (rectCapture.top + amount < rectCapture.bottom)
		rectCapture.top += amount;
}

void PrintScreen::displace_rectCapture_bottom_smart(int amount)
{
	if (rectCapture.bottom + amount > rectCapture.top)
		rectCapture.bottom += amount;
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

void PrintScreen::screenshot_region_and_save(LPCWSTR fileExtension, LPCWSTR mbMessage)
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
	save_rect_on_screen_as_image(rectCapture.left, rectCapture.top,
		rectCapture.right - rectCapture.left, rectCapture.bottom - rectCapture.top, fileName);
	ShowWindow(hwnd1, SW_SHOW);
	ShowWindow(hwnd2, SW_SHOW);
	draw_window();
	MessageBox(0, mbMessage, L"Screen Captured", MB_OK);
}


LRESULT CALLBACK PrintScreen::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_ERASEBKGND:
		break;
	case WM_KEYDOWN:
		thisPtr->key_pressed(wParam);
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
		thisPtr->mouse_wheel(GET_WHEEL_DELTA_WPARAM(wParam) / 120); // Mouse wheel spins are expressed in multiples of 120
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
