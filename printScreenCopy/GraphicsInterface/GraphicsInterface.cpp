#include "stdafx.h"
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
	//graphics = ::new Gdiplus::Graphics(backHdc);
}

void GraphicsInterface::size_change()
{
	RECT rectClient;
	GetClientRect(hwnd, &rectClient);
	
	DeleteObject(backbuffer);
	backbuffer = CreateCompatibleBitmap(hdc, rectClient.right, rectClient.bottom);
	SelectObject(backHdc, backbuffer);
}

			/* Draw */

void GraphicsInterface::draw_rect(int x, int y, int width, int height, int red, int green, int blue, int alpha)
{
	RECT rectDraw;
	rectDraw.left = x;
	rectDraw.right = x + width;
	rectDraw.top = y;
	rectDraw.bottom = y + height;

	//HBRUSH hBrush = CreateSolidBrush(RGB(red, green, blue));
	//FillRect(backHdc, &rectDraw, hBrush);
	//DeleteObject(hBrush);
	Gdiplus::SolidBrush hBrush(Gdiplus::Color(alpha, red, green, blue));
	Gdiplus::Graphics graphics(backHdc);
	graphics.FillRectangle(&hBrush, x, y, width, height);
}

void GraphicsInterface::copy_buffer()
{
	RECT rectClient;
	GetClientRect(hwnd, &rectClient);

	// copy memhdc to hdc
	BitBlt(hdc, 0, 0, rectClient.right, rectClient.bottom, backHdc, 0, 0, SRCCOPY);
	//StretchBlt(hdc, 0, 0, rectClient.right, rectClient.bottom, backHdc, 0, 0, rectClient.right, rectClient.bottom, SRCCOPY);
}