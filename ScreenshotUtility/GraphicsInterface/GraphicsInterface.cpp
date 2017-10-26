#include "GraphicsInterface.h"


GraphicsInterface::GraphicsInterface(HWND p_hwnd)
{
	hwnd = p_hwnd;

	RECT rectClient;
	GetClientRect(hwnd, &rectClient);
	hdc = GetDC(hwnd);
	backHdc = CreateCompatibleDC(hdc);
	backbuffer = CreateCompatibleBitmap(hdc, rectClient.right, rectClient.bottom);
	SelectObject(backHdc, backbuffer);
	graphics = new Gdiplus::Graphics(backHdc);
}

void GraphicsInterface::draw_rect(int x, int y, int width, int height, int red, int green, int blue, int alpha)
{	
	Gdiplus::SolidBrush hBrush(Gdiplus::Color(alpha, red, green, blue));
	graphics->FillRectangle(&hBrush, x, y, width, height);
}

void GraphicsInterface::copy_buffer()
{
	// Copy backHdc to hdc
	RECT rectClient;
	GetClientRect(hwnd, &rectClient);
	BitBlt(hdc, 0, 0, rectClient.right, rectClient.bottom, backHdc, 0, 0, SRCCOPY);
}