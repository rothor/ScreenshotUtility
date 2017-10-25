#pragma once
#include <objidl.h>
#include <gdiplus.h>
//#pragma comment(lib, "gdiplus.lib") // Sometimes needed
//#pragma comment(lib, "Msimg32.lib") // Sometimes needed


class GraphicsInterface
{
public:
	GraphicsInterface(HWND);

private:
	HWND hwnd;
	HDC hdc;
	HDC backHdc;
	HBITMAP backbuffer;
	//Gdiplus::Graphics* graphics;

		/* Draw */
public:
	void size_change();
	void draw_rect(int, int, int, int, int, int, int, int = 0xFF);
	void copy_buffer();
};