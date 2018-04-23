#pragma once
#include "GraphicsInterface\GraphicsInterface.h"
#include "SafeRect\SafeRect.h"
#include "OptionsWindow\OptionsWindow.h"
#include <memory>


/* Class: ScreenshotUtilityApp
*	This class creates the window and runs the app. The message loop
*	should be started by another part of the program.
* * * * */
class ScreenshotUtilityApp
{
public:
	ScreenshotUtilityApp(HINSTANCE);
	
private:
	HINSTANCE hInstance;
	HWND hwndBg; // The background window
	HWND hwndFg; // The foreground window
	std::unique_ptr<GraphicsInterface> giBg;
	std::unique_ptr<GraphicsInterface> giFg;
	SafeRect rectCapture; // This defines the rectangle of the screen that will be captured as the screenshot.
	bool mouseRightButtonLastClicked; // Is 0 if the left buttom was the last mouse button clicked, 1 if the right.
	int maskRed;
	int maskGreen;
	int maskBlue;
	int maskAlpha;
	int maskColorMode;
	std::unique_ptr<OptionsWindow> optionsWindow;

		// Private methods
	void left_click(int xIn, int yIn);
	void right_click(int xIn, int yIn);
	void middle_click();
	void mouse_wheel(int amount);
	void key_pressed(int keyCode);

	void cycle_mask_color_mode();
	void draw_window();
	void screenshot_region_and_save(LPCWSTR fileExtension, LPCWSTR mbMessage);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};