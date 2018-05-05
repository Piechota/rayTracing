#include <stdlib.h>     
#include <time.h>       

#include "headers.h"

#define USE_BVH
#define MULTI_THREAD
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

Vec3* GTexture = nullptr;

TArray< IHitTable* > GCreatedHitTables;
TArray< IMaterial* > GCreatedMaterials;
TArray< ITexture* > GCreatedTextures;

void GetRandomScene( TArray< IHitTable* >& objects )
{
	ITexture* constatTexB = new CConstantTexture( Vec3( 0.2f, 0.3f, 0.1f ) );
	ITexture* constatTexW = new CConstantTexture( Vec3( 0.9f, 0.9f, 0.9f ) );
	ITexture* checker = new CCheckerTexture( constatTexB, constatTexW );

	GCreatedTextures.Add( constatTexB );
	GCreatedTextures.Add( constatTexW );
	GCreatedTextures.Add( checker );

	IMaterial* lambertChecker = new CLambertianMat( checker );
	IHitTable* sphereA = new CSphere( Vec3( 0.f, -1000.f, 0.f ), 1000.f, lambertChecker );

	GCreatedMaterials.Add( lambertChecker );
	GCreatedHitTables.Add( sphereA );

	objects.Add( sphereA );
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
					ITexture* texture = new CConstantTexture( Vec3( Math::Rand()*Math::Rand(), Math::Rand()*Math::Rand(), Math::Rand()*Math::Rand() ) );
					IMaterial* material = new CLambertianMat( texture );
					IHitTable* sphere = new CMovingSphere( center, center + Vec3( 0.f, 0.5f * Math::Rand(), 0.f ), 0.f, 1.f, 0.2f, material );

					GCreatedTextures.Add( texture );
					GCreatedMaterials.Add( material );
					GCreatedHitTables.Add( sphere );

					objects.Add( sphere );
				}
				else if ( matRand < 0.95f )
				{
					IMaterial* material = new CMetalMat( Vec3( 0.5f * ( 1.f + Math::Rand() ), 0.5f * ( 1.f + Math::Rand() ), 0.5f * ( 1.f + Math::Rand() ) ), 0.5f * Math::Rand() );
					IHitTable* sphere = new CSphere( center, 0.2f, material);

					GCreatedMaterials.Add( material );
					GCreatedHitTables.Add( sphere );

					objects.Add( sphere );
				}
				else
				{
					IMaterial* material = new CDielectricMat( 1.5f );
					IHitTable* sphere = new CSphere( center, 0.2f, material);

					GCreatedMaterials.Add( material );
					GCreatedHitTables.Add( sphere );

					objects.Add( sphere );
				}
			}
		}
	}

	{
		IMaterial* material = new CDielectricMat( 1.5f );
		IHitTable* sphere = new CSphere( Vec3( 0.f, 1.f, 0.f ), 1.f, material);

		GCreatedMaterials.Add( material );
		GCreatedHitTables.Add( sphere );

		objects.Add( sphere );
	}

	{
		ITexture* texture = new CConstantTexture( Vec3( 0.4f, 0.2f, 0.1f ) );
		IMaterial* material = new CLambertianMat( texture );
		IHitTable* sphere = new CSphere( Vec3( -4.f, 1.f, 0.f ), 1.f, material);

		GCreatedTextures.Add( texture );
		GCreatedMaterials.Add( material );
		GCreatedHitTables.Add( sphere );

		objects.Add( sphere );
	}

	{
		IMaterial* material = new CMetalMat( Vec3( 0.7f, 0.6f, 0.5f), 0.f );
		IHitTable* sphere = new CSphere( Vec3( 4.f, 1.f, 0.f ), 1.f, material);

		GCreatedMaterials.Add( material );
		GCreatedHitTables.Add( sphere );

		objects.Add( sphere );
	}
}

void GetSimpleScene( TArray< IHitTable* >& objects )
{
	ITexture* image = new CImageTexture( GTexture, 256, 256 );
	ITexture* perlin = new CNoiseTexture( 5.f );
	ITexture* white = new CConstantTexture( Vec3( 15.f, 15.f, 15.f ) );
	IMaterial* material0 = new CLambertianMat( perlin );
	//IMaterial* material1 = new CLambertianMat( image );
	IMaterial* material1 = new CDiffuseLightMat( white );

	IHitTable* sphere0 = new CSphere( Vec3( 0.f, -1000.f, 0.f ), 1000.f, material0 );
	IHitTable* sphere1 = new CSphere( Vec3( 0.f, 2.f, 0.f ), 2.f, material1 );

	GCreatedTextures.Add( image );
	GCreatedTextures.Add( perlin );
	GCreatedTextures.Add( white );
	GCreatedMaterials.Add( material0 );
	GCreatedMaterials.Add( material1 );
	GCreatedHitTables.Add( sphere0 );
	GCreatedHitTables.Add( sphere1 );

	objects.Add( sphere0 );
	objects.Add( sphere1 );
}

void GetCornellBox( TArray< IHitTable* >& objects, CCamera& camera, unsigned int const winWidth, unsigned int const winHeight )
{
	ITexture* redTexture = new CConstantTexture( Vec3( 0.65f, 0.05f, 0.05f ) );
	ITexture* whiteTexture = new CConstantTexture( Vec3( 0.73f, 0.73f, 0.73f ) );
	ITexture* greenTexture = new CConstantTexture( Vec3( 0.12f, 0.45f, 0.15f ) );
	ITexture* lightTexture = new CConstantTexture( Vec3( 15.f, 15.f, 15.f ) );

	IMaterial* redMaterial = new CLambertianMat( redTexture );
	IMaterial* whiteMaterial = new CLambertianMat( whiteTexture );
	IMaterial* greenMaterial = new CLambertianMat( greenTexture );
	IMaterial* lightMaterial = new CDiffuseLightMat( lightTexture );

	IHitTable* rect0 = new CYZRect( 0.f, 555.f, 0.f, 555.f, 555.f, greenMaterial );
	IHitTable* rect1 = new CYZRect( 0.f, 555.f, 0.f, 555.f, 0.f, redMaterial );
	IHitTable* rect2 = new CXZRect( 213.f, 343.f, 227.f, 332.f, 554.f, lightMaterial );
	IHitTable* rect3 = new CXZRect( 0.f, 555.f, 0.f, 555.f, 555.f, whiteMaterial );
	IHitTable* rect4 = new CXZRect( 0.f, 555.f, 0.f, 555.f, 0.f, whiteMaterial );
	IHitTable* rect5 = new CXYRect( 0.f, 555.f, 0.f, 555.f, 555.f, whiteMaterial );

	IHitTable* fRect0 = new CFlipNormals( rect0 );
	IHitTable* fRect3 = new CFlipNormals( rect3 );
	IHitTable* fRect5 = new CFlipNormals( rect5 );

	IHitTable* box0 = new CBox( Vec3( 0.f, 0.f, 0.f ), Vec3( 165.f, 165.f, 165.f ), whiteMaterial );
	IHitTable* box1 = new CBox( Vec3( 0.f, 0.f, 0.f ), Vec3( 165.f, 330.f, 165.f ), whiteMaterial );

	IHitTable* rotate0 = new CRotateY( box0, -18.f );
	IHitTable* rotate1 = new CRotateY( box1, 15.f );

	IHitTable* translate0 = new CTranslate( Vec3( 130.f, 0.f, 65.f ), rotate0 );
	IHitTable* translate1 = new CTranslate( Vec3( 265.f, 0.f, 295.f ), rotate1 );

	GCreatedTextures.Add( redTexture );
	GCreatedTextures.Add( whiteTexture );
	GCreatedTextures.Add( greenTexture );
	GCreatedTextures.Add( lightTexture );

	GCreatedMaterials.Add( redMaterial );
	GCreatedMaterials.Add( whiteMaterial );
	GCreatedMaterials.Add( greenMaterial );
	GCreatedMaterials.Add( lightMaterial );

	GCreatedHitTables.Add(rect0);
	GCreatedHitTables.Add(rect3);
	GCreatedHitTables.Add(rect5);

	GCreatedHitTables.Add(box0);
	GCreatedHitTables.Add(box1);
	GCreatedHitTables.Add(rotate0);
	GCreatedHitTables.Add(rotate1);

	objects.Add( fRect0 );
	objects.Add( rect1 );
	objects.Add( rect2 );
	objects.Add( fRect3 );
	objects.Add( rect4 );
	objects.Add( fRect5 );
	objects.Add( translate0 );
	objects.Add( translate1 );

	Vec3 const lookFrom( 278.f, 278.f, -800.f );
	Vec3 const lookAt( 278.f, 278.f, 0.f );
	float const distanceToFocus = 10.f;
	float const aperture = 0.f;

	camera.Set( lookFrom, lookAt, Vec3(0.f, 1.f, 0.f), winWidth, winHeight, 40.f, aperture, distanceToFocus, 0.f, 1.f ); 
}

void GetCornellBoxSmoke( TArray< IHitTable* >& objects, CCamera& camera, unsigned int const winWidth, unsigned int const winHeight )
{
	ITexture* redTexture = new CConstantTexture( Vec3( 0.65f, 0.05f, 0.05f ) );
	ITexture* whiteTexture = new CConstantTexture( Vec3( 0.73f, 0.73f, 0.73f ) );
	ITexture* greenTexture = new CConstantTexture( Vec3( 0.12f, 0.45f, 0.15f ) );
	ITexture* lightTexture = new CConstantTexture( Vec3( 7.f, 7.f, 7.f ) );

	IMaterial* redMaterial = new CLambertianMat( redTexture );
	IMaterial* whiteMaterial = new CLambertianMat( whiteTexture );
	IMaterial* greenMaterial = new CLambertianMat( greenTexture );
	IMaterial* lightMaterial = new CDiffuseLightMat( lightTexture );

	IHitTable* rect0 = new CYZRect( 0.f, 555.f, 0.f, 555.f, 555.f, greenMaterial );
	IHitTable* rect1 = new CYZRect( 0.f, 555.f, 0.f, 555.f, 0.f, redMaterial );
	IHitTable* rect2 = new CXZRect( 113.f, 443.f, 127.f, 432.f, 554.f, lightMaterial );
	IHitTable* rect3 = new CXZRect( 0.f, 555.f, 0.f, 555.f, 555.f, whiteMaterial );
	IHitTable* rect4 = new CXZRect( 0.f, 555.f, 0.f, 555.f, 0.f, whiteMaterial );
	IHitTable* rect5 = new CXYRect( 0.f, 555.f, 0.f, 555.f, 555.f, whiteMaterial );

	IHitTable* fRect0 = new CFlipNormals( rect0 );
	IHitTable* fRect3 = new CFlipNormals( rect3 );
	IHitTable* fRect5 = new CFlipNormals( rect5 );

	IHitTable* box0 = new CBox( Vec3( 0.f, 0.f, 0.f ), Vec3( 165.f, 165.f, 165.f ), whiteMaterial );
	IHitTable* box1 = new CBox( Vec3( 0.f, 0.f, 0.f ), Vec3( 165.f, 330.f, 165.f ), whiteMaterial );

	IHitTable* rotate0 = new CRotateY( box0, -18.f );
	IHitTable* rotate1 = new CRotateY( box1, 15.f );

	IHitTable* translate0 = new CTranslate( Vec3( 130.f, 0.f, 65.f ), rotate0 );
	IHitTable* translate1 = new CTranslate( Vec3( 265.f, 0.f, 295.f ), rotate1 );

	ITexture* oneTexture = new CConstantTexture( Vec3( 1.f, 1.f, 1.f ) );
	ITexture* zeroTexture = new CConstantTexture( Vec3( 0.f, 0.f, 0.f ) );

	IMaterial* iso0 = new CIsotropicMat( oneTexture );
	IMaterial* iso1 = new CIsotropicMat( zeroTexture );

	IHitTable* smoke0 = new CConstantMedium( translate0, 0.01f, iso0 );
	IHitTable* smoke1 = new CConstantMedium( translate1, 0.01f, iso1 );

	IMaterial* die = new CDielectricMat( 1.5f );
	IHitTable* sphere = new CSphere( Vec3( 278.f, 278.f, 0.f ), 50.f, die );
	IHitTable* smoke2 = new CConstantMedium( sphere, 0.2f, iso0 );

	GCreatedTextures.Add( redTexture );
	GCreatedTextures.Add( whiteTexture );
	GCreatedTextures.Add( greenTexture );
	GCreatedTextures.Add( lightTexture );
	GCreatedTextures.Add( oneTexture );
	GCreatedTextures.Add( zeroTexture );

	GCreatedMaterials.Add( redMaterial );
	GCreatedMaterials.Add( whiteMaterial );
	GCreatedMaterials.Add( greenMaterial );
	GCreatedMaterials.Add( lightMaterial );
	GCreatedMaterials.Add( iso0 );
	GCreatedMaterials.Add( iso1 );
	GCreatedMaterials.Add( die );

	GCreatedHitTables.Add(rect0);
	GCreatedHitTables.Add(rect3);
	GCreatedHitTables.Add(rect5);

	GCreatedHitTables.Add(box0);
	GCreatedHitTables.Add(box1);
	GCreatedHitTables.Add(rotate0);
	GCreatedHitTables.Add(rotate1);
	GCreatedHitTables.Add(translate0);
	GCreatedHitTables.Add(translate1);

	objects.Add( fRect0 );
	objects.Add( rect1 );
	objects.Add( rect2 );
	objects.Add( fRect3 );
	objects.Add( rect4 );
	objects.Add( fRect5 );
	objects.Add( smoke0 );
	objects.Add( smoke1 );
	objects.Add( sphere );
	objects.Add( smoke2 );

	Vec3 const lookFrom( 278.f, 278.f, -800.f );
	Vec3 const lookAt( 278.f, 278.f, 0.f );
	float const distanceToFocus = 10.f;
	float const aperture = 0.f;

	camera.Set( lookFrom, lookAt, Vec3(0.f, 1.f, 0.f), winWidth, winHeight, 40.f, aperture, distanceToFocus, 0.f, 1.f ); 
}

void GenerateTexture()
{
	GTexture = new Vec3[ 256 * 256 ];
	for ( unsigned int y = 0; y < 256; ++y )
	{
		for ( unsigned int x = 0; x < 256; ++x )
		{
			float const u = float( x ) * ( 1.f / 255.f );
			float const v = float( y ) * ( 1.f / 255.f );
			GTexture[ y * 256 + x ].Set( 0.5f * cosf(50.f * u * MathConsts::PI) + 0.5f, 0.5f * cosf(50.f * v * MathConsts::PI) + 0.5f, 0.f );
		}
	}
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

	GenerateTexture();

	CCamera camera;

#ifdef USE_BVH
	CBVHNode* sceneBVH = nullptr;
	{
		TArray< IHitTable* > objects;
		//GetRandomScene( objects );
		//GetSimpleScene( objects );
		GetCornellBoxSmoke( objects, camera, winWidth, winHeight );
		sceneBVH = new CBVHNode( objects.Data(), int( objects.Size() ), 0.f, 1.f );
	}
#else 
	CHitTableList hitTableList;
	GetRandomScene( hitTableList.GetList() );
#endif

	/*Vec3 const lookFrom( 13.f, 2.f, 3.f );
	Vec3 const lookAt( 0.f, 0.f, 0.f );
	float const distanceToFocus = 10.f;
	float const aperture = 0.f;

	CCamera camera( lookFrom, lookAt, Vec3(0.f, 1.f, 0.f), winWidth, winHeight, 45.f, aperture, distanceToFocus, 0.f, 1.f );*/
	

#ifdef MULTI_THREAD
#	ifdef USE_BVH
	GRender.DrawMT( camera, sceneBVH );
#	else
	GRender.DrawMT( camera, &hitTableList );
#	endif
#else
#	ifdef USE_BVH
	GRender.Draw( camera, sceneBVH );
#	else
	GRender.Draw( camera, &hitTableList );
#	endif
#endif

	SetWindowText( hwnd, "RayTracing (finished)" );

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

	{
		unsigned int const num = GCreatedHitTables.Size();
		for ( unsigned int i = 0; i < num; ++i )
		{
			delete GCreatedHitTables[ i ];
		}
	}
	{
		unsigned int const num = GCreatedMaterials.Size();
		for ( unsigned int i = 0; i < num; ++i )
		{
			delete GCreatedMaterials[ i ];
		}
	}
	{
		unsigned int const num = GCreatedTextures.Size();
		for ( unsigned int i = 0; i < num; ++i )
		{
			delete GCreatedTextures[ i ];
		}
	}

	delete GTexture;

	GRender.Release();
	return 0;
}