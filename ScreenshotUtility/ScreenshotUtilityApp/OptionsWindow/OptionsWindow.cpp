#include "OptionsWindow.h"
#include "../BasicWindowMaker/BasicWindowMaker.h"
#include "custom message defines.h"
#include <string>

#define ID_LB_WINDOWS		0
#define ID_BN_REFRESH		1


OptionsWindow::OptionsWindow(HWND hwndIn, HINSTANCE hInstIn)
{
	optionsHwnd_ = 0;
	mainHwnd_ = hwndIn;
	hInst_ = hInstIn;
	lbWindowsHwnd_ = 0;
	bnRefreshHwnd_ = 0;
}

void OptionsWindow::show()
{
	if (optionsHwnd_)
		ShowWindow(optionsHwnd_, SW_SHOW);
	// Create the window if it hasn't already been
	else
		createWindow();
}

void OptionsWindow::hide()
{
	if (optionsHwnd_)
		ShowWindow(optionsHwnd_, SW_HIDE);
}

LRESULT CALLBACK OptionsWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	OptionsWindow* pOW = (OptionsWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
			pOW->hide();
	}
	break;
	case WM_KEYUP:
		break;
	case WM_COMMAND:
	{
		int controlId = LOWORD(wParam);
		int controlMsg = HIWORD(wParam);

		switch (controlId)
		{
		case ID_LB_WINDOWS:
		{
			switch (controlMsg)
			{
			case LBN_SELCHANGE:
			{
				int curSel{ (int)SendMessage(pOW->lbWindowsHwnd_, LB_GETCURSEL, 0, 0) };
				SendMessage(pOW->mainHwnd_, MSG_WINDOWPICKED, 0, (LPARAM)pOW->windowList[curSel]);
			}
			}
		}
		break;
		case ID_BN_REFRESH:
		{
			switch (controlMsg)
			{
			case BN_CLICKED:
			{
				pOW->loadWindowList();
			}
			}
		}
		break;
		}
	}
	break;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MBUTTONDOWN:
		break;
	case WM_MOUSEWHEEL:
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_ERASEBKGND:
		break;
	case WM_CLOSE:
		pOW->hide();
		break;
	case WM_DESTROY:
		pOW->optionsHwnd_ = 0;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void OptionsWindow::createWindow()
{
	// Create options window
	int exWindowStyles = WS_EX_TOOLWINDOW;
	int windowStyles = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | 0 | WS_CLIPCHILDREN;
	int classStyles = 0;
	optionsHwnd_ = BasicWindowMaker::createWindow(
		L"Options", L"optionsWindowClass",
		300, 0, 430, 240,
		WndProc, hInst_, SW_SHOW, exWindowStyles, windowStyles, classStyles,
		0, 0, 0, mainHwnd_
	);
	SetWindowLongPtr(optionsHwnd_, GWLP_USERDATA, (LONG_PTR)this);
	
	// Create the windows listbox
	lbWindowsHwnd_ = BasicWindowMaker::createBasicLb(
		optionsHwnd_, hInst_, 10, 10, 395, 150,
		L"hi", LBS_NOTIFY | LBS_DISABLENOSCROLL | WS_VSCROLL | WS_HSCROLL, ID_LB_WINDOWS
	);
	SendMessage(lbWindowsHwnd_, LB_SETHORIZONTALEXTENT, 800, NULL); // Allow horizontal scrolling for 800 units
	loadWindowList();

	// Create the refresh button
	bnRefreshHwnd_ = BasicWindowMaker::createBasicBtn(
		optionsHwnd_, hInst_, 320, 165, 85, 25,
		L"Refresh list", 0, ID_BN_REFRESH
	);
}

void OptionsWindow::loadWindowList()
{
	SendMessage(lbWindowsHwnd_, LB_RESETCONTENT, 0, 0);
	EnumWindows(enumWindowsProc, reinterpret_cast<LPARAM>(this));
}

BOOL CALLBACK OptionsWindow::enumWindowsProc(HWND hWnd, LPARAM lParam)
{
	OptionsWindow* pOW{ reinterpret_cast<OptionsWindow*>(lParam) };

	// Skip if window isn't visible
	if (!IsWindowVisible(hWnd))
		return true;

	// Get the window name from the hWnd
	int nameLength = GetWindowTextLength(hWnd);
	if (nameLength == 0) // Skip if window doesn't have name, or is one of this program's windows
		return true;
	std::wstring windowName;
	windowName.resize(nameLength);
	int actualNameLength{ GetWindowText(hWnd, &windowName[0], nameLength + 1) };
	if (actualNameLength != nameLength)
		windowName.resize(actualNameLength);

	// Skip if is one of this program's windows
	if (windowName.compare(L"Screenshot Tool") == 0 ||
		windowName.compare(L"Screenshot Tool extra") == 0 ||
		windowName.compare(L"Options") == 0
		)
		return true;

	// Add hwnd to list, and add window name to list box
	pOW->windowList.push_back(hWnd);
	SendMessage(pOW->lbWindowsHwnd_, LB_ADDSTRING, NULL, (LPARAM)(&windowName[0]));

	return true;
}