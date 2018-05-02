#include "headers.h"

void CRender::Init(unsigned int winWidth, unsigned int winHeight)
{
	m_winWidth = winWidth;
	m_winHeight = winHeight;

	m_bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bitmapinfo.bmiHeader.biWidth = m_winWidth;
	m_bitmapinfo.bmiHeader.biHeight = m_winHeight;
	m_bitmapinfo.bmiHeader.biPlanes = 1;
	m_bitmapinfo.bmiHeader.biBitCount = 32;
	m_bitmapinfo.bmiHeader.biCompression = BI_RGB;
	m_bitmapinfo.bmiHeader.biSizeImage = 0;
	m_bitmapinfo.bmiHeader.biClrUsed = 0;
	m_bitmapinfo.bmiHeader.biClrImportant = 0;

	m_backbuffer = new SPixel[winWidth * winHeight];
}

void CRender::Draw()
{
	Present();
}

void CRender::Present()
{
	HDC hDC = GetDC(m_hwnd);
	SetDIBitsToDevice(hDC, 0, 0, m_winWidth, m_winHeight, 0, 0, 0, m_winHeight, m_backbuffer, &m_bitmapinfo, DIB_RGB_COLORS);
	ReleaseDC(m_hwnd, hDC);
}

void CRender::Release()
{
	delete[] m_backbuffer;
}

CRender GRender;