#include <thread>
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

Vec3 Color( CRay const& ray, IHitTable const* const pScene, unsigned int depth )
{
	SHitInfo hitInfo;
	if ( pScene->Hit( ray, 0.001f, FLT_MAX, hitInfo ) )
	{
		CRay rayScattered;
		Vec3 attenuation;
		Vec3 const emitted = hitInfo.m_material->Emitted( hitInfo.m_position, hitInfo.m_uv );

		if ( depth < 100 && hitInfo.m_material->Scatter( ray, hitInfo, attenuation, rayScattered ) )
		{
			return emitted + attenuation * Color( rayScattered, pScene, depth + 1 );
		}
		else
		{
			return emitted;
		}
	}
	else
	{
		return Vec3( 0.f, 0.f, 0.f );
	}
}

struct SThreadContext
{
	CCamera const* const m_pCamera;
	IHitTable const* const m_pScene;
	unsigned int const m_winWidth;
	unsigned int const m_winHeight;
	SPixel* const m_pBackbuffer;
};

void DrawThread( SThreadContext const& context, unsigned int startPixel, unsigned int const endPixel )
{
	Vec3 const origin = Vec3( 0.f, 0.f, 0.f );

	float const invWidth = 1.f / float( context.m_winWidth );
	float const invHeight = 1.f / float( context.m_winHeight );

	for ( ; startPixel < endPixel; ++startPixel )
	{
		unsigned int const x = startPixel % context.m_winWidth;
		unsigned int const y = startPixel / context.m_winWidth;

		Vec3 color( 0.f, 0.f, 0.f );

		unsigned int const samplesNum = 500;
		for ( unsigned int s = 0; s < samplesNum; ++s )
		{
			float const offsetU = Math::Rand() * 0.9999f;
			float const offsetV = Math::Rand() * 0.9999f;

			float const u = ( float( x ) + offsetU ) * invWidth;
			float const v = ( float( y ) + offsetV ) * invHeight;
			CRay const ray = context.m_pCamera->GetRay( u, v );
			color += Color( ray, context.m_pScene, 0 );
		}

		color *= ( 1.f / float(samplesNum) );
		color.x = sqrtf( color.x );
		color.y = sqrtf( color.y );
		color.z = sqrtf( color.z );

		context.m_pBackbuffer[ y * context.m_winWidth + x ] = SPixel::ToPixel( color );
	}
}

void CRender::DrawMT( CCamera const& camera, IHitTable const* const pScene )
{
	SThreadContext const threadContext = { &camera, pScene, m_winWidth, m_winHeight, m_backbuffer };

	unsigned int const threadNum = std::thread::hardware_concurrency();
	unsigned int const createdThreadNum = threadNum - 1;
	std::thread* threads = new std::thread[ createdThreadNum ];

	unsigned int const allPixelsNum = m_winHeight * m_winWidth;
	unsigned int const pixelsNumPerThread = allPixelsNum / threadNum;

	unsigned int const firstThreadPixelsNum = pixelsNumPerThread + ( allPixelsNum % pixelsNumPerThread );
	unsigned int threadID = 0;
	for ( unsigned int i = firstThreadPixelsNum; i < allPixelsNum; i += pixelsNumPerThread )
	{
		threads[ threadID ] = std::thread( DrawThread, std::ref( threadContext ), i, i + pixelsNumPerThread );
		++threadID;
	}

	DrawThread( std::ref( threadContext ), 0, firstThreadPixelsNum );

	for ( unsigned int i = 0; i < createdThreadNum; ++i )
	{
		threads[ i ].join();
	}

	delete[] threads;

	Present();
}

void CRender::Draw( CCamera const& camera, IHitTable const* const pScene )
{
	Vec3 const origin = Vec3( 0.f, 0.f, 0.f );

	float const invWidth = 1.f / float( m_winWidth );
	float const invHeight = 1.f / float( m_winHeight );

	for ( unsigned int y = 0; y < m_winHeight; ++y )
	{
		for ( unsigned int x = 0; x < m_winWidth; ++x )
		{
			Vec3 color( 0.f, 0.f, 0.f );

			unsigned int const samplesNum = 100;
			for ( unsigned int s = 0; s < samplesNum; ++s )
			{
				float const offsetU = Math::Rand() * 0.9999f;
				float const offsetV = Math::Rand() * 0.9999f;

				float const u = ( float( x ) + offsetU ) * invWidth;
				float const v = ( float( y ) + offsetV ) * invHeight;
				CRay const ray = camera.GetRay( u, v );
				color += Color( ray, pScene, 0 );
			}

			color *= ( 1.f / float(samplesNum) );
			color.x = sqrtf( color.x );
			color.y = sqrtf( color.y );
			color.z = sqrtf( color.z );

			m_backbuffer[ y * m_winWidth + x ] = SPixel::ToPixel( color );
			//Present();

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