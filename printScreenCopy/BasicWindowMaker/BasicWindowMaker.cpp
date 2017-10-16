#include "stdafx.h"
#include "BasicWindowMaker.h"

int BasicWindowMaker::maxLoadstring = 100;


BasicWindowMaker::BasicWindowMaker()
{
	
}

HWND BasicWindowMaker::create_window(wchar_t* p_title, wchar_t* p_windowClass, int p_x, int p_y, int p_width,
	int p_height, WNDPROC p_wndProc, HINSTANCE p_hInst, int p_nCmdShow, int exWindowStyles, int windowStyles, int classStyles,
	int menuRc, int iconRc, int iconSmallRc, HWND parent)
{
	//LoadStringW(p_hInst, IDS_APP_TITLE, p_title, maxLoadstring); //note: I am commenting these two lines because an error occurs if I don't, and also because I don't know what these lines do. I may need to fix these lines if a problem occurs in the future involving the name and class of the window.
	//LoadStringW(p_hInst, IDC_AMAPGAME2, p_windowClass, maxLoadstring);
	MyRegisterClass(p_hInst, p_wndProc, p_windowClass, classStyles, iconRc, iconSmallRc, menuRc);

	return InitInstance(p_title, p_windowClass, p_x, p_y, p_width, p_height, p_hInst, exWindowStyles, windowStyles, p_nCmdShow, parent);
}

ATOM BasicWindowMaker::MyRegisterClass(HINSTANCE hInstance, WNDPROC p_wndProc, wchar_t* p_windowClass,
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

HWND BasicWindowMaker::InitInstance(wchar_t* p_title, wchar_t* p_windowClass, int p_x, int p_y, int p_width,
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
