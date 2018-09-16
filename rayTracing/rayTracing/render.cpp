#include <thread>
#include <atomic>
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

struct SDrawThreadContext
{
	CCamera const* const m_pCamera;
	IHitTable const* const m_pScene;
	unsigned int const m_winWidth;
	unsigned int const m_winHeight;
	unsigned int const m_sampleNum;
	float* const m_pBackbuffer;
};

unsigned int constexpr SampleNum = 100;
std::atomic_uint SampleID( 0 );
void DrawThread( SDrawThreadContext const& context )
{
	unsigned int sampleID = SampleID.fetch_add( 1 );
	float const invWidth = 1.f / float( context.m_winWidth );
	float const invHeight = 1.f / float( context.m_winHeight );

	while ( sampleID < context.m_sampleNum )
	{
		Vec3 color( 0.f, 0.f, 0.f );

		#if USE_BLUE_NOISE
			Vec2 const sample = Math::BlueNoise2D(Math::WangHash(sampleID));

			float const offsetU = sample.x;
			float const offsetV = sample.y;
		#else
			float const offsetU = Math::Rand() * 0.9999f;
			float const offsetV = Math::Rand() * 0.9999f;
		#endif

		unsigned int const pixelID = sampleID / SampleNum;
		float const x = float(pixelID % context.m_winWidth);
		float const y = float(pixelID / context.m_winWidth);
		float const u = ( x + offsetU ) * invWidth;
		float const v = ( y + offsetV ) * invHeight;

		CRay const ray = context.m_pCamera->GetRay( u, v );
		color += Color( ray, context.m_pScene, 0 );

		context.m_pBackbuffer[ ( sampleID * 3 ) + 0 ] = color.x;
		context.m_pBackbuffer[ ( sampleID * 3 ) + 1 ] = color.y;
		context.m_pBackbuffer[ ( sampleID * 3 ) + 2 ] = color.z;

		sampleID = SampleID.fetch_add( 1 );
	}
}

struct SCombineThreadContext
{
	SPixel* const m_pBackbuffer;
	float const* const m_pSrcBackbuffer;
};
void CombineThread( SCombineThreadContext const& context, unsigned int const start, unsigned int const end )
{
	float const invSampleNum = 1.f / float( SampleNum );
	for ( unsigned int i = start; i < end; ++i )
	{
		float r = 0.f;
		float g = 0.f;
		float b = 0.f;

		unsigned int const pixelOffset = i * SampleNum;
		for ( unsigned int s = 0; s < SampleNum; ++s )
		{
			r += context.m_pSrcBackbuffer[ 3 * ( pixelOffset + s ) + 0 ];
			g += context.m_pSrcBackbuffer[ 3 * ( pixelOffset + s ) + 1 ];
			b += context.m_pSrcBackbuffer[ 3 * ( pixelOffset + s ) + 2 ];
		}
		r *= invSampleNum;
		g *= invSampleNum;
		b *= invSampleNum;

		r = sqrtf( r );
		g = sqrtf( g );
		b = sqrtf( b );

		context.m_pBackbuffer[ i ] = SPixel::ToPixel( Vec3( r, g, b ) );
	}
}

void CRender::DrawMT( CCamera const& camera, IHitTable const* const pScene )
{
	float* fBackbuffer = new float[ m_winWidth * m_winHeight * SampleNum * 3 ];
	SDrawThreadContext const drawThreadContext = { &camera, pScene, m_winWidth, m_winHeight, m_winWidth * m_winHeight * SampleNum, fBackbuffer };
	SCombineThreadContext const combineThreadContext = { m_backbuffer, fBackbuffer };

	unsigned int const threadNum = std::thread::hardware_concurrency();
	unsigned int const createdThreadNum = threadNum - 1;
	std::thread* threads = new std::thread[ createdThreadNum ];

	for ( unsigned int i = 0; i < createdThreadNum; ++i )
	{
		threads[ i ] = std::thread( DrawThread, std::ref( drawThreadContext ) );
	}
	DrawThread( drawThreadContext );

	for ( unsigned int i = 0; i < createdThreadNum; ++i )
	{
		threads[ i ].join();
	}

	unsigned int const allPixelsNum = m_winHeight * m_winWidth;
	unsigned int const pixelsNumPerThread = allPixelsNum / threadNum;
	unsigned int const firstThreadPixelsNum = pixelsNumPerThread + ( allPixelsNum % pixelsNumPerThread );
	unsigned int threadID = 0;
	for ( unsigned int i = firstThreadPixelsNum; i < allPixelsNum; i += pixelsNumPerThread )
	{
		threads[ threadID ] = std::thread( CombineThread, std::ref( combineThreadContext ), i, i + pixelsNumPerThread );
		++threadID;
	}
	CombineThread( combineThreadContext, 0, firstThreadPixelsNum );

	for ( unsigned int i = 0; i < createdThreadNum; ++i )
	{
		threads[ i ].join();
	}

	delete[] threads;
	delete[] fBackbuffer;

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
				#if USE_BLUE_NOISE
					Vec2 const sample = Math::BlueNoise2D(Math::WangHash(s));

					float const offsetU = sample.x;
					float const offsetV = sample.y;
				#else
					float const offsetU = Math::Rand() * 0.9999f;
					float const offsetV = Math::Rand() * 0.9999f;
				#endif

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