#include <Windows.h>
#include <windowsx.h>
#include "BasicWindowMaker.h"

ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC p_wndProc, wchar_t* p_windowClass,
	int classStyles, int iconRc, int iconSmallRc, int menuRc);
HWND InitInstance(wchar_t* p_title, wchar_t* p_windowClass, int p_x, int p_y, int p_width,
	int p_height, HINSTANCE p_hInstance, int exWindowStyles, int windowStyles, int p_nCmdShow, HWND parent);


HWND BasicWindowMaker::createWindow(wchar_t* p_title, wchar_t* p_windowClass, int p_x, int p_y, int p_width,
	int p_height, WNDPROC p_wndProc, HINSTANCE p_hInst, int p_nCmdShow, int exWindowStyles, int windowStyles, int classStyles,
	int menuRc, int iconRc, int iconSmallRc, HWND parent)
{
	MyRegisterClass(p_hInst, p_wndProc, p_windowClass, classStyles, iconRc, iconSmallRc, menuRc);

	return InitInstance(p_title, p_windowClass, p_x, p_y, p_width, p_height, p_hInst, exWindowStyles, windowStyles, p_nCmdShow, parent);
}

HWND BasicWindowMaker::createBasicBtn(HWND parentHwnd, HINSTANCE hInst, int x, int y, int w, int h, LPCWSTR text,
	int extraStyles, int identifier)
{
	HWND hwndBtn = CreateWindowExW(NULL, L"BUTTON", text,
		BS_FLAT | WS_GROUP | WS_BORDER | WS_VISIBLE | WS_CHILD + extraStyles,
		x, y, w, h, parentHwnd, (HMENU)identifier, hInst, NULL);

	return hwndBtn;
}

HWND BasicWindowMaker::createBasicLb(HWND parentHwnd, HINSTANCE hInst, int x, int y, int w, int h, LPCWSTR text,
	int extraStyles, int identifier)
{
	HWND hwndLb = CreateWindowExW(NULL, L"LISTBOX", text,
		WS_GROUP | WS_BORDER | WS_VISIBLE | WS_CHILD + extraStyles,
		x, y, w, h, parentHwnd, (HMENU)identifier, hInst, NULL);

	return hwndLb;
}

HWND BasicWindowMaker::createBasicEc(HWND parentHwnd, HINSTANCE hInst, int x, int y, int w, int h, LPCWSTR text, int extraStyles
	, int identifier)
{
	//ES_MULTILINE, ES_READONLY, ES_NUMBER
	HWND hwndEC = CreateWindowExW(NULL, L"EDIT", text,
		WS_GROUP | WS_BORDER | WS_VISIBLE | WS_CHILD + extraStyles,
		x, y, w, h, parentHwnd, (HMENU)identifier, hInst, NULL);
	//Edit_LimitText(hwndEC, 2048);
	Edit_CanUndo(hwndEC);

	return hwndEC;
}


ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC p_wndProc, wchar_t* p_windowClass,
	int classStyles, int iconRc, int iconSmallRc, int menuRc)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = classStyles;
	wcex.lpfnWndProc = p_wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = DLGWINDOWEXTRA;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(iconRc));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(menuRc);
	wcex.lpszClassName = p_windowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iconSmallRc));

	return RegisterClassExW(&wcex);
}

HWND InitInstance(wchar_t* p_title, wchar_t* p_windowClass, int p_x, int p_y, int p_width,
	int p_height, HINSTANCE p_hInstance, int exWindowStyles, int windowStyles, int p_nCmdShow, HWND parent)
{
	HWND hwnd = CreateWindowExW(exWindowStyles, p_windowClass, p_title, windowStyles,
		p_x, p_y, p_width, p_height, parent, nullptr, p_hInstance, nullptr);

	if (!hwnd)
		return 0;

	ShowWindow(hwnd, p_nCmdShow);
	UpdateWindow(hwnd);

	return hwnd;
}
