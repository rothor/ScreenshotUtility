#pragma once
#include <objidl.h>
#include <gdiplus.h>
//#pragma comment(lib, "gdiplus.lib") // Sometimes needed
//#pragma comment(lib, "Msimg32.lib") // Sometimes needed

/* Class: GraphicsInterface
 * Purpose:
 *     This class handles drawing to a window with GDI+ using double buffering. It acts as an interface
 *     for drawing to a window.
 *
 * Usage:
 *     Instantiate a GraphicsInterface object by passing the HWND of the window it will draw to as an
 *     argument. The "copy_buffer" method refers to the fact that this class draws using double buffering.
 *     So use the "draw_rect" method to draw to the buffer context, and when you are done drawing call the
 *     "copy_buffer" method to copy the entire drawing to the actual window.
 *     This class expects GDI+ to have already been setup.
 * * * */
class GraphicsInterface
{
public:
	GraphicsInterface(HWND);

private:
	HWND hwnd;
	HDC hdc;
	HDC backHdc;
	HBITMAP backbuffer;
	Gdiplus::Graphics* graphics;

public:
	void draw_rect(int x, int y, int width, int height, int red, int green, int blue, int alpha = 0xFF);
	void copy_buffer();
};