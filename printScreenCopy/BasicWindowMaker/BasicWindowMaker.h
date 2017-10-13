#pragma once


class BasicWindowMaker
{
public:
	BasicWindowMaker() {}

private:
	HWND mainHwnd{ NULL };
	int maxLoadstring{ 100 };
	typedef LRESULT (*typeWndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC p_wndProc, wchar_t* p_windowClass,
		int classStyles, int iconRc, int iconSmallRc, int menuRc);
	BOOL InitInstance(wchar_t* p_title, wchar_t* p_windowClass, int p_x, int p_y, int p_width,
		int p_height, HINSTANCE p_hInstance, int exWindowStyles, int windowStyles, int p_nCmdShow, HWND);

public:
	bool create_window(wchar_t* p_title, wchar_t* p_windowClass, int p_x, int p_y, int p_width,
		int p_height, WNDPROC p_wndProc, HINSTANCE p_hInst, int p_nCmdShow = 5, int exWindowStyles = 0, int windowStyles = 0,
		int classStyles = 3, int menuRc = 0, int iconRc = 0, int = 0, HWND = NULL);
	HWND get_hwnd() { return mainHwnd; }
};