#pragma once


class MyPrintScreen
{
public:
	MyPrintScreen() {}
	int get_width() { return rectCaptureRight - rectCaptureLeft; }
	int get_height() { return rectCaptureBottom - rectCaptureTop; }

private:
	HWND hWnd;
	int rectCaptureLeft{ 100 };
	int rectCaptureTop{ 200 };
	int rectCaptureRight{ 900 };
	int rectCaptureBottom{ 400 };
	bool lrClickFocus{ false };

	void set_rcl_offset_smart(int xP) { if (rectCaptureLeft + xP < rectCaptureRight) rectCaptureLeft += xP; }
	void set_rcr_offset_smart(int xP) { if (rectCaptureRight + xP > rectCaptureLeft) rectCaptureRight += xP; }
	void set_rct_offset_smart(int yP) { if (rectCaptureTop + yP < rectCaptureBottom) rectCaptureTop += yP; }
	void set_rcb_offset_smart(int yP) { if (rectCaptureBottom + yP > rectCaptureTop) rectCaptureBottom += yP; }

	void save_square_on_screen_as_file(LPCTSTR fileExtension);
	void capture_image_and_save(LPCWSTR fileExtension, LPCWSTR mbMessage);

public:
	void set_hwnd(HWND hWndP);
	void redraw_window();
	void repaint_scene();
	void left_click(int xP, int yP);
	void right_click(int xP, int yP);
	void up_arrow();
	void down_arrow();
	void left_arrow();
	void right_arrow();
	void pressed_g();
	void pressed_j();
	void pressed_p();
	void pressed_b();
};