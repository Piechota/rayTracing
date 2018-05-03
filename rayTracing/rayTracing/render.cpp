#include "headers.h"
#include <float.h>

Vec3 Color( CRay const& ray, CHitTableList const& hitTableList, unsigned int depth )
{
	SHitInfo hitInfo;
	if ( hitTableList.Hit( ray, 0.001f, FLT_MAX, hitInfo ) )
	{
		CRay rayScattered;
		Vec3 attenuation;

		if ( depth < 50 && hitInfo.m_material->Scatter( ray, hitInfo, attenuation, rayScattered ) )
		{
			return attenuation * Color( rayScattered, hitTableList, depth + 1 );
		}
		else
		{
			return Vec3( 0.f, 0.f, 0.f );
		}
	}
	else
	{
		Vec3 const direction = ray.GetDirection().GetNormalized();
		float const t = fabsf( 0.5f * direction.y + 0.5f );
		return Vec3( 1.f, 1.f, 1.f ) * ( 1.f - t ) + Vec3( 0.5f, 0.7f, 1.f ) * t;
	}
}

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

void CRender::Draw( CCamera const& camera, CHitTableList const& hitTableList )
{
	Vec3 const origin = Vec3( 0.f, 0.f, 0.f );
	

	float const invWidth = 1.f / float( m_winWidth );
	float const invHeight = 1.f / float( m_winHeight );

	for ( unsigned int y = 0; y < m_winHeight; ++y )
	{
		for ( unsigned int x = 0; x < m_winWidth; ++x )
		{
			Vec3 color( 0.f, 0.f, 0.f );

			for ( unsigned int s = 0; s < 4; ++s )
			{
				float offsetU = ( s & 1 ) ? 0.f : 0.5f;
				float offsetV = ( ( s >> 1 ) & 1 ) ? 0.f : 0.5f;

				float const u = ( float( x ) + offsetU ) * invWidth;
				float const v = ( float( y ) + offsetV ) * invHeight;
				CRay const ray = camera.GetRay( u, v );
				color += Color( ray, hitTableList, 0 );
			}

			color *= ( 1.f / 4.f );

			m_backbuffer[ y * m_winWidth + x ] = SPixel::ToPixel( color );
		}
	}

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