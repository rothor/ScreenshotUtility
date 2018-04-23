#include "targetver.h"
#include "Resource.h"
#include "ScreenshotUtilityApp\ScreenshotUtilityApp.h"
#include <tchar.h> // Needed by WinMain


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{	
	// Create ScreenshotUtilityApp instance
	ScreenshotUtilityApp sua(hInstance);

	// Enter the message loop
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
	
	return msg.wParam;
}
