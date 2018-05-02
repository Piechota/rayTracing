#pragma once

class CRender
{
private:
	BITMAPINFO m_bitmapinfo;
	SPixel* m_backbuffer;
	HWND m_hwnd;
	unsigned int m_winWidth;
	unsigned int m_winHeight;

private:
	void Present();

public:
	void Init(unsigned int winWidth, unsigned int winHeight);
	void SetHWnd( HWND hwnd ) { m_hwnd = hwnd; }

	void Draw();
	void Release();
};
extern CRender GRender;