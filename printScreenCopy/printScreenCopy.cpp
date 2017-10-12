#include "stdafx.h"
#include "printScreenCopy.h"
#include "PrintScreen.h"
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
#include <windowsx.h>


#define MAX_LOADSTRING 100

// Global Variables
HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

MyPrintScreen printScreen;

// Forward declarations
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PRINTSCREENCOPY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PRINTSCREENCOPY));
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_PRINTSCREENCOPY));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(0);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance;

	hWnd = CreateWindowEx(WS_EX_LAYERED, szWindowClass, szTitle, WS_POPUP,
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) - 1, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	printScreen.set_hwnd(hWnd);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		{
			SetLayeredWindowAttributes(hWnd, 0, 100, LWA_ALPHA);
		}
		break;
	case WM_PAINT:
		{
			printScreen.repaint_scene();
		}
		break;
	case WM_KEYDOWN:
		{
			if (wParam == VK_DOWN)
			{
				printScreen.down_arrow();
				RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
			}
			else if (wParam == VK_UP)
			{
				printScreen.up_arrow();
				RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
			}
			else if (wParam == VK_RIGHT)
			{
				printScreen.right_arrow();
				RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
			}
			else if (wParam == VK_LEFT)
			{
				printScreen.left_arrow();
				RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
			}
			else if (wParam == 0x42) //b
			{
				printScreen.pressed_b();
			}
			else if (wParam == 0x47) //g
			{
				printScreen.pressed_g();
			}
			else if (wParam == 0x4A) //j
			{
				printScreen.pressed_j();
			}
			else if (wParam == 0x50) //p
			{
				printScreen.pressed_p();
			}
			else if (wParam == VK_ESCAPE)
			{
				PostQuitMessage(0);
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			printScreen.left_click(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			printScreen.right_click(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
