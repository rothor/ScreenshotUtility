#include "GraphicsInterface.h"


template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL) {
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

int intFromRGB(int red, int green, int blue)
{
	return red * 0x10000 + green * 0x100 + blue;
}

GraphicsInterface::GraphicsInterface(HWND p_hwnd, int drawWidthIn, int drawHeightIn) :
	m_pDirect2dFactory(0),
	m_pRenderTarget(0),
	m_pBrush(0)
{
	hwnd = p_hwnd;
	drawWidth = drawWidthIn;
	drawHeight = drawHeightIn;

	// Create Direct2D factory
	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_pDirect2dFactory);

	// Create a Direct2D render target.
	D2D1_SIZE_U size = D2D1::SizeU(drawWidthIn, drawHeightIn);
	ID2D1HwndRenderTarget* pRenderTarget;
	m_pDirect2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size),
		&m_pRenderTarget
	);

	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	m_pRenderTarget->BeginDraw();
}

GraphicsInterface::~GraphicsInterface()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pBrush);
}

void GraphicsInterface::size_change(int newDrawWidth, int newDrawHeight)
{
	drawWidth = newDrawWidth;
	drawHeight = newDrawHeight;

	//x
}

int GraphicsInterface::getWidth()
{
	return drawWidth;
}

int GraphicsInterface::getHeight()
{
	return drawHeight;
}


void GraphicsInterface::draw_rect(int x, int y, int width, int height, int red, int green, int blue, double alpha)
{
	D2D1_RECT_F rectangle1 = D2D1::RectF(x, y, x + width, y + height);
	SafeRelease(&m_pBrush);
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(intFromRGB(red, green, blue), 1)),
		&m_pBrush
	);
	m_pRenderTarget->FillRectangle(&rectangle1, m_pBrush);
}

void GraphicsInterface::copy_buffer()
{
	// EndDraw() coppies the offscreen buffer to the window
	m_pRenderTarget->EndDraw();
	m_pRenderTarget->BeginDraw();
}