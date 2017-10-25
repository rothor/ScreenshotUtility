#include "ScreenshotUtility.h"
#include "PrintScreen.h"
#include "Resource.h"
#include <tchar.h>


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// Startup GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	// Create PrintScreen instance
	PrintScreen printScreen(hInstance);

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCREENSHOTUTILITY));
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
