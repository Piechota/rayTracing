#include <stdlib.h>     
#include <time.h>       

#include "headers.h"

#define MULTI_THREAD
#define DUMMY_PROFILER
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

#ifdef DUMMY_PROFILER
	GTimer.Tick();
	int64_t prof = GTimer.TimeFromStart();
#endif

	CBVHNode* sceneBVH = nullptr;
	{
		TArray< IHitTable* > objects;
		GetCornellBoxSmoke( objects, camera, winWidth, winHeight );
		sceneBVH = new CBVHNode( objects.Data(), int( objects.Size() ), 0.f, 1.f );
	}

#ifdef MULTI_THREAD
	GRender.DrawMT( camera, sceneBVH );
#else
	GRender.Draw( camera, sceneBVH );
#endif

#ifdef DUMMY_PROFILER
	prof = GTimer.TimeFromStart() - prof;
	char log[ 1024 ];
	sprintf_s( log, ARRAYSIZE(log), "total time: %lli\nseconds: %f", prof, GTimer.GetSeconds( prof ) );
	OutputDebugStringA( log );
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
	}

	delete sceneBVH;

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