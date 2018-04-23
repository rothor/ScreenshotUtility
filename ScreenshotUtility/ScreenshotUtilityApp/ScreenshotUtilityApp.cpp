#include "ScreenshotUtilityApp.h"
#include "BasicWindowMaker\BasicWindowMaker.h"
#include "Screenshotter\Screenshotter.h"
#include "..\Resource.h"
#include "OptionsWindow\custom message defines.h"
#include <windowsx.h> // For GET_X_LPARAM and GET_Y_LPARAM macros
#include <string>


ScreenshotUtilityApp::ScreenshotUtilityApp(HINSTANCE hInstanceIn)
{
		// Initialize fields
	const int C_DEFAULT_WINDOW_2_ALPHA = 0x90;
	maskAlpha = C_DEFAULT_WINDOW_2_ALPHA;
	hInstance = hInstanceIn;
	mouseRightButtonLastClicked = false;
	maskRed = 0;
	maskGreen = 0;
	maskBlue = 0;
	maskColorMode = 0;

		// Create window 1 (background window)
	int exWindowStyles = WS_EX_LAYERED;
	int windowStyles = WS_POPUP; // No border or menu
	int classStyles = 0;
	int windowWidth{ GetSystemMetrics(SM_CXSCREEN) };
	int windowHeight{ GetSystemMetrics(SM_CYSCREEN) };
	hwndBg = BasicWindowMaker::createWindow(
		L"Screenshot Tool", L"screenshotToolClass",
		0, 0, windowWidth, windowHeight + 1,
		WndProc, hInstance, SW_SHOW, exWindowStyles, windowStyles, classStyles,
		0, IDI_SCREENSHOTUTILITY, IDI_SMALL
	);
	giBg = std::unique_ptr<GraphicsInterface>(
		new GraphicsInterface(hwndBg, windowWidth, windowHeight)
	);
	// Window 1 will have an alpha of 0x01, so it will be almost invisible, but it will still capture mouse clicks.
	SetLayeredWindowAttributes(hwndBg, 0, 0x01, LWA_ALPHA);
	SetWindowLongPtr(hwndBg, GWLP_USERDATA, (LONG_PTR)this);

		// Create window 2 (foreground)
	exWindowStyles = WS_EX_LAYERED | WS_EX_TRANSPARENT;
	windowStyles = WS_POPUP; // No border or menu
	classStyles = 0;
	hwndFg = BasicWindowMaker::createWindow(
		L"Screenshot Tool extra", L"screenshotToolClass",
		0, 0, windowWidth, windowHeight + 1,
		WndProc, hInstance, SW_SHOW, exWindowStyles, windowStyles, classStyles,
		0, IDI_SCREENSHOTUTILITY, IDI_SMALL, hwndBg // the last argument makes window 2 a child of window 1
	);
	giFg = std::unique_ptr<GraphicsInterface>(
		new GraphicsInterface(hwndFg, windowWidth, windowHeight)
	);
	SetLayeredWindowAttributes(hwndFg, RGB(1, 1, 1), maskAlpha, LWA_ALPHA | LWA_COLORKEY);
	SetWindowLongPtr(hwndFg, GWLP_USERDATA, (LONG_PTR)this);
	
		// Other initializations
	rectCapture.setLeft(giBg->getWidth() / 2 - giBg->getWidth() / 6);
	rectCapture.setRight(giBg->getWidth() / 2 + giBg->getWidth() / 6);
	rectCapture.setTop(giBg->getHeight() / 2 - giBg->getHeight() / 5);
	rectCapture.setBottom(giBg->getHeight() / 2 + giBg->getHeight() / 5);
	optionsWindow = std::unique_ptr<OptionsWindow>(
		new OptionsWindow(hwndBg, hInstance)
	);
	
	draw_window();
}

// This method controls what happens when the user left clicks.
void ScreenshotUtilityApp::left_click(int xIn, int yIn)
{
	mouseRightButtonLastClicked = false;
	rectCapture.setLeft(xIn);
	rectCapture.setTop(yIn);

	draw_window();
}

// This method controls what happens when the user right clicks.
void ScreenshotUtilityApp::right_click(int xIn, int yIn)
{
	mouseRightButtonLastClicked = true;
	rectCapture.setRight(xIn);
	rectCapture.setBottom(yIn);

	draw_window();
}

// This method controls what happens when the user middle clicks.
void ScreenshotUtilityApp::middle_click()
{
	cycle_mask_color_mode();

	draw_window();
}

// This method controls what happens when the user mouse wheels.
void ScreenshotUtilityApp::mouse_wheel(int amount)
{
	// Calculate the window 2's new alpha
	maskAlpha += amount * 0x10;
	// Make sure alpha is not greater than 0xFF
	if (maskAlpha > 0xFF)
		maskAlpha = 0xFF;
	// Make sure alpha is not less than 0
	else if (maskAlpha < 0)
		maskAlpha = 0;

	// Set the window's alpha
	SetLayeredWindowAttributes(hwndFg, RGB(1, 1, 1), maskAlpha, LWA_ALPHA | LWA_COLORKEY);
}

// This method controls what happens when the user presses a keyboard key.
void ScreenshotUtilityApp::key_pressed(int keyCode)
{
	if (keyCode == VK_DOWN)
	{
		if (!mouseRightButtonLastClicked)
			rectCapture.moveTop(1);
		else
			rectCapture.moveBottom(1);

		draw_window();
	}
	else if (keyCode == VK_UP)
	{
		if (!mouseRightButtonLastClicked)
			rectCapture.moveTop(-1);
		else
			rectCapture.moveBottom(-1);

		draw_window();
	}
	else if (keyCode == VK_RIGHT)
	{
		if (!mouseRightButtonLastClicked)
			rectCapture.moveLeft(1);
		else
			rectCapture.moveRight(1);

		draw_window();
	}
	else if (keyCode == VK_LEFT)
	{
		if (!mouseRightButtonLastClicked)
			rectCapture.moveLeft(-1);
		else
			rectCapture.moveRight(-1);

		draw_window();
	}
	else if (keyCode == VK_F1)
		optionsWindow->show();
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

void ScreenshotUtilityApp::cycle_mask_color_mode()
{
	maskColorMode++;
	if (maskColorMode > 3)
		maskColorMode = 0;

	if (maskColorMode == 0)
	{
		maskRed = 0;
		maskGreen = 0;
		maskBlue = 0;
	}
	else if (maskColorMode == 1)
	{
		maskRed = 0xFF;
		maskGreen = 0xFF;
		maskBlue = 0xFF;
	}
	else if (maskColorMode == 2)
	{
		maskRed = 0x00;
		maskGreen = 0xE0;
		maskBlue = 0xF0;
	}
	else if (maskColorMode == 3)
	{
		maskRed = 0xFF;
		maskGreen = 0x00;
		maskBlue = 0x00;
	}
}

// This method repaints the windows.
void ScreenshotUtilityApp::draw_window()
{
	// Draw window 1
	giBg->draw_rect(0, 0, giBg->getWidth(), giBg->getHeight(), 0xFF, 0xFF, 0xFF);
	giBg->copy_buffer();

	// Draw window 2
	giFg->draw_rect(0, 0, giFg->getWidth(), giFg->getHeight(), maskRed, maskGreen, maskBlue);
	giFg->draw_rect(rectCapture.getLeft(), rectCapture.getTop(),
		rectCapture.getWidth(), rectCapture.getHeight(), 1, 1, 1);
	giFg->copy_buffer();
}

// This method saves a screenshot from the rectangle defined by rectCapture.
void ScreenshotUtilityApp::screenshot_region_and_save(LPCWSTR fileExtension, LPCWSTR mbMessage)
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

	optionsWindow->hide();
	ShowWindow(hwndBg, SW_HIDE);
	Screenshotter::takeScreenshot(rectCapture.getLeft(), rectCapture.getTop(),
		rectCapture.getWidth(), rectCapture.getHeight(), fileName);
	ShowWindow(hwndBg, SW_SHOW);
	MessageBox(0, mbMessage, L"Screen Captured", MB_OK);
}

LRESULT CALLBACK ScreenshotUtilityApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ScreenshotUtilityApp* screenshotUtilityApp = (ScreenshotUtilityApp*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message)
	{
	case MSG_WINDOWPICKED:
		HWND outsideHwnd;
		outsideHwnd = reinterpret_cast<HWND>(lParam);
		RECT winRect;
		GetWindowRect(outsideHwnd, &winRect);
		screenshotUtilityApp->rectCapture.setLeft(winRect.left);
		screenshotUtilityApp->rectCapture.setRight(winRect.right);
		screenshotUtilityApp->rectCapture.setTop(winRect.top);
		screenshotUtilityApp->rectCapture.setBottom(winRect.bottom);
		screenshotUtilityApp->draw_window();
		break;
	case WM_PAINT: // This message must be handled like this so that message boxes are properly painted.
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_ERASEBKGND: // This message must be handled to prevent a minor graphical error.
		break;
	case WM_KEYDOWN:
		screenshotUtilityApp->key_pressed(wParam);
		break;
	case WM_LBUTTONDOWN:
		screenshotUtilityApp->left_click(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_RBUTTONDOWN:
		screenshotUtilityApp->right_click(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MBUTTONDOWN:
		screenshotUtilityApp->middle_click();
		break;
	case WM_MOUSEWHEEL:
		screenshotUtilityApp->mouse_wheel(GET_WHEEL_DELTA_WPARAM(wParam) / 120); // Mouse wheel spins are expressed in multiples of 120.
		break;
	case WM_SIZE: // This message doesn't need to be handled. The window should never resize.
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
