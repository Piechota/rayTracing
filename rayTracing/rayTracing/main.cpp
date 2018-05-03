#include <stdlib.h>     
#include <time.h>       

#include "headers.h"

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

	CHitTableList hitTableList;
	hitTableList.AddHitTable( new CSphere( Vec3( 0.f, 0.f, -1.f ), 0.5f, new CLambertianMat( Vec3(0.8f, 0.3f, 0.3f ) ) ) );
	hitTableList.AddHitTable( new CSphere( Vec3( 0.f, -100.5f, -1.f ), 100.f, new CLambertianMat( Vec3( 0.8f, 0.8f, 0.0f ) ) ) );
	hitTableList.AddHitTable( new CSphere( Vec3( 1.f, 0.f, -1.f ), 0.5f, new CMetalMat( Vec3( 0.8f, 0.6f, 0.2f ), 0.3f ) ) );
	hitTableList.AddHitTable( new CSphere( Vec3( -1.f, 0.f, -1.f ), 0.5f, new CDielectricMat( 1.5f ) ) );

	Vec3 const lookFrom( 3.f, 3.f, 2.f );
	Vec3 const lookAt( 0.f, 0.f, -1.f );
	float const distanceToFocus = ( lookFrom - lookAt ).Magnitude();
	float const aperture = 2.f;

	CCamera camera( lookFrom, lookAt, Vec3(0.f, 1.f, 0.f), winWidth, winHeight, 20.f, aperture, distanceToFocus );

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

		GRender.Draw( camera, hitTableList );
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

	hitTableList.Clear();
	GRender.Release();
	return 0;
}