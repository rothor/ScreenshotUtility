#pragma once
#include "GraphicsInterface\GraphicsInterface.h"
#include "BasicWindowMaker\BasicWindowMaker.h"


class PrintScreen
{
public:
	PrintScreen(HINSTANCE);

	// Fields
private:
	HINSTANCE hInstance;
	GraphicsInterface* gi1;
	GraphicsInterface* gi2;
	HWND hwnd1;
	HWND hwnd2;
	RECT rectCapture; // This defines the rectangle of the screen that will be captured as the screenshot.
	bool mouseRightButtonLastClicked; // Is 0 if the left buttom was the last mouse button clicked, 1 if the right.
	int maskR;
	int maskG;
	int maskB;
	int maskA;
	int maskColorMode;
	static PrintScreen* thisPtr; // Used exclusively by the WndProc method.

	// Public methods (these mostly serve to notify the class of user input)
public:
	void left_click(int xIn, int yIn);
	void right_click(int xIn, int yIn);
	void middle_click();
	void mouse_wheel(int amount);
	void key_pressed(int keyCode);

	// Private methods
private:
	void set_rectCapture_topLeft_safe(int xIn, int yIn);
	void set_rectCapture_bottomRight_safe(int xIn, int yIn);
	void displace_rectCapture_left_safe(int amount);
	void displace_rectCapture_right_safe(int amount);
	void displace_rectCapture_top_safe(int amount);
	void displace_rectCapture_bottom_safe(int amount);
	void cycle_mask_color_mode();
	void draw_window();
	void screenshot_region_and_save(LPCWSTR fileExtension, LPCWSTR mbMessage);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};