#pragma once


class BasicWindowMaker
{
public:
	static HWND createWindow(wchar_t* p_title, wchar_t* p_windowClass, int p_x, int p_y, int p_width,
		int p_height, WNDPROC p_wndProc, HINSTANCE p_hInst, int p_nCmdShow = 5, int exWindowStyles = 0, int windowStyles = 0,
		int classStyles = 3, int menuRc = 0, int iconRc = 0, int = 0, HWND = NULL);
	static HWND createBasicBtn(HWND parentHwnd, HINSTANCE hInst, int x, int y, int w, int h, LPCWSTR text,
		int extraStyles = 0, int identifier = NULL);
	static HWND createBasicLb(HWND parentHwnd, HINSTANCE hInst, int x, int y, int w, int h, LPCWSTR text,
		int extraStyles = 0, int identifier = NULL);
	static HWND createBasicEc(HWND parentHwnd, HINSTANCE hInst, int x, int y, int w, int h, LPCWSTR text, int extraStyles = 0
		, int identifier = NULL);

private:
	typedef LRESULT(*typeWndProc)(HWND, UINT, WPARAM, LPARAM);
};