#pragma once
#include "ray.h"
#include "camera.h"
#include "hitTable.h"
#include "volumetric.h"
#include "sphere.h"
#include "movingSphere.h"
#include "rect.h"
#include "helpHitTables.h"
#include "hitTableList.h"
#include "box.h"
#include "bvhNode.h"
#include "material.h"

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

	void Draw( CCamera const& camera, IHitTable const* const pScene );
	void DrawMT( CCamera const& camera, IHitTable const* const pScene );
	void Release();
};
extern CRender GRender;