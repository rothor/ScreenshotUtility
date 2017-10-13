#pragma once
#include "GraphicsInterface\GraphicsInterface.h"
#include "BasicWindowMaker\BasicWindowMaker.h"


class MyPrintScreen
{
public:
	MyPrintScreen();

	// Fields
private:
	GraphicsInterface* gi1;
	GraphicsInterface* gi2;
	BasicWindowMaker bwm1;
	BasicWindowMaker bwm2;
	int rectCaptureLeft;
	int rectCaptureTop;
	int rectCaptureRight;
	int rectCaptureBottom;
	bool lrClickFocus;
	int maskR;
	int maskG;
	int maskB;
	int maskA;

	// Public methods
public:
	void size_change();
	void left_click(int, int);
	void right_click(int, int);
	void middle_click();
	void mouse_wheel(int);
	void up_arrow();
	void down_arrow();
	void left_arrow();
	void right_arrow();
	void draw_window();
	void pressed_g();
	void pressed_j();
	void pressed_p();
	void pressed_b();

	// Private methods
private:
	void set_rcl_offset_smart(int);
	void set_rcr_offset_smart(int);
	void set_rct_offset_smart(int);
	void set_rcb_offset_smart(int);
	int get_width();
	int get_height();
	void screenshot_region_and_save(LPCWSTR, LPCWSTR);
};