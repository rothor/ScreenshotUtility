#include "stdafx.h"
#include "printScreenCopy.h"
#include "PrintScreen.h"


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// Startup GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	// Create MyPrintScreen instance
	MyPrintScreen printScreen(hInstance);

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

	// Shutdown GDI+
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return msg.wParam;
}
