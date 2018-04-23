#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <memory>
#pragma comment(lib, "d2d1")


class GraphicsInterface
{
public:
	GraphicsInterface(HWND, int drawWidth, int drawHeight);
	~GraphicsInterface();

	void size_change(int newDrawWidth, int newDrawHeight);
	void draw_rect(int x, int y, int width, int height, int red, int green, int blue, double alpha = 1);
	void copy_buffer();
	int getWidth();
	int getHeight();

private:
	HWND hwnd;
	int drawWidth;
	int drawHeight;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pBrush;
};