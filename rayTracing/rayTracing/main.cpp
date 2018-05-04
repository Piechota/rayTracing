#include <stdlib.h>     
#include <time.h>       

#include "headers.h"

#define USE_BVH
//#define DUMMY_PROFILER
#ifdef DUMMY_PROFILER
#include <stdio.h>
long constexpr GFrameProfNum = 2000;
int64_t GFramesProf[ GFrameProfNum ];
long GFrameProfID = 0;
#endif

CInputManager GInputManager;
CSystemInput GSystemInput;
CTimer GTimer;

void GetRandomScene( CHitTableList& outHitTable )
{
	outHitTable.AddHitTable( new CSphere( Vec3( 0.f, -1000.f, 0.f ), 1000.f, new CLambertianMat( Vec3( 0.7f, 0.7f, 0.7f ) ) ) );
	for ( int a = -10; a < 10; ++a )
	{
		for ( int b = -10; b < 10; ++b )
		{
			float const matRand = Math::Rand();
			Vec3 const center( a + 0.9f * Math::Rand(), 0.2f, b + 0.9f * Math::Rand() );
			if ( 0.9f < ( center - Vec3( 4.f, 0.2f, 0.f ) ).Magnitude() )
			{
				if ( matRand < 0.8f )
				{
					outHitTable.AddHitTable( new CMovingSphere( center, center + Vec3( 0.f, 0.5f * Math::Rand(), 0.f ), 0.f, 1.f, 0.2f, new CLambertianMat( Vec3( Math::Rand()*Math::Rand(), Math::Rand()*Math::Rand(), Math::Rand()*Math::Rand() ) ) ) );
				}
				else if ( matRand < 0.95f )
				{
					outHitTable.AddHitTable( new CSphere( center, 0.2f, new CMetalMat( Vec3( 0.5f * ( 1.f + Math::Rand() ), 0.5f * ( 1.f + Math::Rand() ), 0.5f * ( 1.f + Math::Rand() ) ), 0.5f * Math::Rand() ) ) );
				}
				else
				{
					outHitTable.AddHitTable( new CSphere( center, 0.2f, new CDielectricMat( 1.5f ) ) );
				}
			}
		}
	}

	outHitTable.AddHitTable( new CSphere( Vec3( 0.f, 1.f, 0.f ), 1.f, new CDielectricMat( 1.5f ) ) );
	outHitTable.AddHitTable( new CSphere( Vec3( -4.f, 1.f, 0.f ), 1.f, new CLambertianMat( Vec3( 0.4f, 0.2f, 0.1f ) ) ) );
	outHitTable.AddHitTable( new CSphere( Vec3( 4.f, 1.f, 0.f ), 1.f, new CMetalMat( Vec3( 0.7f, 0.6f, 0.5f), 0.f ) ) );
}

void GetRandomScene( CBVHNode*& outBVHNode )
{
	TArray< IHitTable* > objects;

	objects.Add( new CSphere( Vec3( 0.f, -1000.f, 0.f ), 1000.f, new CLambertianMat( Vec3( 0.7f, 0.7f, 0.7f ) ) ) );
	for ( int a = -10; a < 10; ++a )
	{
		for ( int b = -10; b < 10; ++b )
		{
			float const matRand = Math::Rand();
			Vec3 const center( a + 0.9f * Math::Rand(), 0.2f, b + 0.9f * Math::Rand() );
			if ( 0.9f < ( center - Vec3( 4.f, 0.2f, 0.f ) ).Magnitude() )
			{
				if ( matRand < 0.8f )
				{
					objects.Add( new CMovingSphere( center, center + Vec3( 0.f, 0.5f * Math::Rand(), 0.f ), 0.f, 1.f, 0.2f, new CLambertianMat( Vec3( Math::Rand()*Math::Rand(), Math::Rand()*Math::Rand(), Math::Rand()*Math::Rand() ) ) ) );
				}
				else if ( matRand < 0.95f )
				{
					objects.Add( new CSphere( center, 0.2f, new CMetalMat( Vec3( 0.5f * ( 1.f + Math::Rand() ), 0.5f * ( 1.f + Math::Rand() ), 0.5f * ( 1.f + Math::Rand() ) ), 0.5f * Math::Rand() ) ) );
				}
				else
				{
					objects.Add( new CSphere( center, 0.2f, new CDielectricMat( 1.5f ) ) );
				}
			}
		}
	}

	objects.Add( new CSphere( Vec3( 0.f, 1.f, 0.f ), 1.f, new CDielectricMat( 1.5f ) ) );
	objects.Add( new CSphere( Vec3( -4.f, 1.f, 0.f ), 1.f, new CLambertianMat( Vec3( 0.4f, 0.2f, 0.1f ) ) ) );
	objects.Add( new CSphere( Vec3( 4.f, 1.f, 0.f ), 1.f, new CMetalMat( Vec3( 0.7f, 0.6f, 0.5f), 0.f ) ) );

	outBVHNode = new CBVHNode( objects.Data(), int( objects.Size() ), 0.f, 1.f );
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, INT nCmdShow)
{
	srand (time(NULL));

	unsigned int winWidth = 600u;
	unsigned int winHeight = 600u;

	WNDCLASS windowClass = { 0 };

	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = CInputManager::WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = "WindowClass";
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	if (!RegisterClass(&windowClass))
	{
		MessageBox(0, "RegisterClass FAILED", 0, 0);
	}

	HWND hwnd = CreateWindow(
		"WindowClass",
		"RayTracing",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		winWidth, winHeight,
		NULL, NULL,
		hInstance,
		NULL);

	if (hwnd == 0)
	{
		MessageBox(0, "CreateWindow FAILED", 0, 0);
	}

	ShowWindow(hwnd, nCmdShow);

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	winWidth = clientRect.right - clientRect.left;
	winHeight = clientRect.bottom - clientRect.top;

	GRender.Init( winWidth, winHeight );
	GRender.SetHWnd(hwnd);
	GInputManager.SetHWnd(hwnd);

	GInputManager.Init();
	GInputManager.AddObserver(&GSystemInput);

#ifdef USE_BVH
	CBVHNode* sceneBVH = nullptr;
	GetRandomScene( sceneBVH );
#else 
	CHitTableList hitTableList;
	GetRandomScene( hitTableList );
#endif

	Vec3 const lookFrom( 13.f, 2.f, 3.f );
	Vec3 const lookAt( 0.f, 0.f, 0.f );
	float const distanceToFocus = ( lookFrom - lookAt ).Magnitude();
	float const aperture = 0.f;

	CCamera camera( lookFrom, lookAt, Vec3(0.f, 1.f, 0.f), winWidth, winHeight, 90.f, aperture, distanceToFocus, 0.f, 1.f );

#ifdef USE_BVH
	GRender.Draw( camera, sceneBVH );
#else
	GRender.Draw( camera, hitTableList );
#endif

	MSG msg = { 0 };
	bool run = true;
	float const axis[] = { 1.f, 0.f, 0.f };
	while (run)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				run = false;
				break;
			}
		}
		GTimer.Tick();
#ifdef DUMMY_PROFILER
		GFramesProf[ GFrameProfID % GFrameProfNum] = GTimer.LastDelta();
		++GFrameProfID;
#endif
	}

#ifdef DUMMY_PROFILER
	int const start = max( 0, GFrameProfID - GFrameProfNum );
	int64_t iMin = INT64_MAX, iMax = 0, iSum = 0;
	for ( int i = start; i < GFrameProfID; ++i )
	{
		INT64 const time = GFramesProf[ i % GFrameProfNum ];
		iMin = min( time, iMin );
		iMax = max( time, iMax );
		iSum += time;
	}

	char log[ 1024 ];
	sprintf_s( log, ARRAYSIZE(log), "frame num: %li\nmin\tmax\tavg\n%lli\t%lli\t%lli\n%fs\t%fs\t%fs\n", GFrameProfID - start, iMin, iMax, iSum / ( GFrameProfID - start ), GTimer.GetSeconds( iMin ), GTimer.GetSeconds( iMax ), GTimer.GetSeconds( iSum / ( GFrameProfID - start ) ) );
	OutputDebugStringA( log );
#endif

#ifdef USE_BVH
	delete sceneBVH;
#else
	hitTableList.Clear();
#endif
	GRender.Release();
	return 0;
}