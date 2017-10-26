#pragma once
#include <Windows.h>

/* Class BasicWindowMaker
 * Purpose:
 *     This class is used to create a window.
 *
 * Usage:
 *     This class is static and does not need to be instantiated. Call the method "create_window" to create a
 *     window. It will return the HWND of the created window on success, or 0 on failure.
 * * * * */
class BasicWindowMaker
{
private:
	typedef LRESULT (*typeWndProc)(HWND, UINT, WPARAM, LPARAM);
	static ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC p_wndProc, wchar_t* p_windowClass,
		int classStyles, int iconRc, int iconSmallRc, int menuRc);
	static HWND InitInstance(wchar_t* p_title, wchar_t* p_windowClass, int p_x, int p_y, int p_width,
		int p_height, HINSTANCE p_hInstance, int exWindowStyles, int windowStyles, int p_nCmdShow, HWND parent);

public:
	static HWND create_window(wchar_t* p_title, wchar_t* p_windowClass, int p_x, int p_y, int p_width,
		int p_height, WNDPROC p_wndProc, HINSTANCE p_hInst, int p_nCmdShow = 5, int exWindowStyles = 0,
		int windowStyles = 0, int classStyles = 3, int menuRc = 0, int iconRc = 0, int = 0, HWND parent = NULL);
};