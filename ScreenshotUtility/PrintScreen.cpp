#include "PrintScreen.h"
#include "GraphicsInterface\GraphicsInterface.h"
#include "Resource.h"
#include <windowsx.h> // For GET_X_LPARAM and GET_Y_LPARAM macros
#include <string>
#include <atlimage.h> // For Cimage class

PrintScreen* PrintScreen::thisPtr = nullptr;


/* Class: PrintScreen
 * Purpose:
 *     This class handles most of the functionality of this program.
 *
 * Usage:
 *     This class should be instantiated only once, before the program enters the message loop. The instance should be
 *     declared in WinMain so that it will not be destroyed until the program terminates.
 * * * * */
PrintScreen::PrintScreen(HINSTANCE hInstanceIn)
{
	// Initialize fields
	thisPtr = this;
	const int C_DEFAULT_WINDOW_2_ALPHA = 0x90;
	maskA = C_DEFAULT_WINDOW_2_ALPHA;
	hInstance = hInstanceIn;
	mouseRightButtonLastClicked = false;
	maskR = 0;
	maskG = 0;
	maskB = 0;
	maskColorMode = 0;

	// Create window 1 (background window)
	int exWindowStyles = WS_EX_LAYERED;
	int windowStyles = WS_POPUP;
	int classStyles = 0;
	hwnd1 = BasicWindowMaker::create_window(L"Screenshot Tool", L"screenshotToolClass",
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) + 1,
		WndProc, hInstance, SW_SHOW, exWindowStyles, windowStyles, classStyles,
		0, IDI_SCREENSHOTUTILITY, IDI_SMALL);
	gi1 = new GraphicsInterface(hwnd1);
	// Window 1 will have an alpha of 0x01, so it will be almost invisible, but it will still capture mouse clicks.
	SetLayeredWindowAttributes(hwnd1, 0, 0x01, LWA_ALPHA);

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
	
	// Initialize rectCapture (this must be initialized after windows are created)
	RECT rectClient;
	GetClientRect(hwnd1, &rectClient);
	rectCapture.left = rectClient.right / 2 - 150;
	rectCapture.right = rectClient.right / 2 + 150;
	rectCapture.top = rectClient.bottom / 2 - 150;
	rectCapture.bottom = rectClient.bottom / 2 + 150;
	
	draw_window();
}


			// Public methods

// This method controls what happens when the user left clicks.
void PrintScreen::left_click(int xIn, int yIn)
{
	set_rectCapture_topLeft_safe(xIn, yIn);

	draw_window();
}

// This method controls what happens when the user right clicks.
void PrintScreen::right_click(int xIn, int yIn)
{
	set_rectCapture_bottomRight_safe(xIn, yIn);

	draw_window();
}

// This method controls what happens when the user middle clicks.
void PrintScreen::middle_click()
{
	cycle_mask_color_mode();

	draw_window();
}

// This method controls what happens when the user mouse wheels.
void PrintScreen::mouse_wheel(int amount)
{
	// Calculate the window 2's new alpha
	maskA += amount * 0x10;
	// Make sure alpha is not greater than 0xFF
	if (maskA > 0xFF)
		maskA = 0xFF;
	// Also make sure alpha is not less than 0
	else if (maskA < 0)
		maskA = 0;

	// Set the window's alpha
	SetLayeredWindowAttributes(hwnd2, RGB(1, 1, 1), maskA, LWA_ALPHA | LWA_COLORKEY);
}

// This method controls what happens when the user presses a keyboard key.
void PrintScreen::key_pressed(int keyCode)
{
	if (keyCode == VK_DOWN)
	{
		if (!mouseRightButtonLastClicked)
			displace_rectCapture_top_safe(1);
		else
			displace_rectCapture_bottom_safe(1);

		draw_window();
	}
	else if (keyCode == VK_UP)
	{
		if (!mouseRightButtonLastClicked)
			displace_rectCapture_top_safe(-1);
		else
			displace_rectCapture_bottom_safe(-1);

		draw_window();
	}
	else if (keyCode == VK_RIGHT)
	{
		if (!mouseRightButtonLastClicked)
			displace_rectCapture_left_safe(1);
		else
			displace_rectCapture_right_safe(1);

		draw_window();
	}
	else if (keyCode == VK_LEFT)
	{
		if (!mouseRightButtonLastClicked)
			displace_rectCapture_left_safe(-1);
		else
			displace_rectCapture_right_safe(-1);

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


			// Private methods

void PrintScreen::set_rectCapture_topLeft_safe(int xIn, int yIn)
{
	mouseRightButtonLastClicked = false;

	// Set rectCapture.left
	rectCapture.left = xIn;
	// Ensure that rectCapture.left is less than rectCapture.right
	if (rectCapture.left >= rectCapture.right)
		rectCapture.right = rectCapture.left + 50;

	// Set rectCapture.top
	rectCapture.top = yIn;
	// Ensure that rectCapture.top is less than rectCapture.bottom
	if (rectCapture.top >= rectCapture.bottom)
		rectCapture.bottom = rectCapture.top + 50;
}

void PrintScreen::set_rectCapture_bottomRight_safe(int xIn, int yIn)
{
	mouseRightButtonLastClicked = true;

	// Set rectCapture.right
	rectCapture.right = xIn;
	// Ensure that rectCapture.right is greater than rectCapture.left
	if (rectCapture.right <= rectCapture.left)
		rectCapture.left = rectCapture.right - 50;

	// Set rectCapture.bottom
	rectCapture.bottom = yIn;
	// Ensure that rectCapture.bottom is greater than rectCapture.top
	if (rectCapture.bottom <= rectCapture.top)
		rectCapture.top = rectCapture.bottom - 50;
}

void PrintScreen::displace_rectCapture_left_safe(int amount)
{
	// Displace rectCapture.left
	rectCapture.left += amount;
	// Ensure that rectCapture.left is less than rectCapture.right
	if (rectCapture.left >= rectCapture.right)
		rectCapture.left = rectCapture.right - 1;
}

void PrintScreen::displace_rectCapture_right_safe(int amount)
{
	// Displace rectCapture.right
	rectCapture.right += amount;
	// Ensure that rectCapture.right is greater than rectCapture.left
	if (rectCapture.right <= rectCapture.left)
		rectCapture.right = rectCapture.left + 1;
}

void PrintScreen::displace_rectCapture_top_safe(int amount)
{
	// Set rectCapture.top
	rectCapture.top += amount;
	// Ensure that rectCapture.top is less than rectCapture.bottom
	if (rectCapture.top >= rectCapture.bottom)
		rectCapture.top = rectCapture.bottom - 1;
}

void PrintScreen::displace_rectCapture_bottom_safe(int amount)
{
	// Set rectCapture.bottom
	rectCapture.bottom += amount;
	// Ensure that rectCapture.bottom is greater than rectCapture.top
	if (rectCapture.bottom <= rectCapture.top)
		rectCapture.bottom = rectCapture.top + 1;
}

void PrintScreen::cycle_mask_color_mode()
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
}

// This method repaints the windows.
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

// This method creates a screenshot from the rectangle defined by rectCapture and saves it as an image.
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
	case WM_PAINT: // This message must be handled like this so that message boxes are properly painted.
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_ERASEBKGND: // This message must be handled to prevent a minor graphical glitch.
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
		thisPtr->mouse_wheel(GET_WHEEL_DELTA_WPARAM(wParam) / 120); // Mouse wheel spins are expressed in multiples of 120.
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
