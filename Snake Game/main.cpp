
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include "Sprite.h"
#include "Board.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

#define SCREEN_WIDTH		800
#define SCREEN_HEIGHT		600

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = nullptr;
HWND                    g_hWnd = nullptr;

ID2D1Factory* g_pD2DFactory = nullptr;
ID2D1HwndRenderTarget* g_pRenderTarget = nullptr;

IWICImagingFactory* g_pWICFactory = nullptr;
Board* board;
Player* player;

ID2D1Bitmap* g_pBoard = nullptr;
ID2D1Bitmap* g_pPlayerBitmap = nullptr;
ID2D1Bitmap* g_pAppleBitmap = nullptr;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT InitDevice(void);
void CleanupDevice(void);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void Render(void);

HRESULT LoadBitmapFromFile(PCWSTR _wcFileName, ID2D1Bitmap** _ppBitmap);



template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(InitWindow(hInstance, nCmdShow)))
		return 0;

	if (FAILED(InitDevice()))
	{
		CleanupDevice();
		return 0;
	}

	LoadBitmapFromFile(L"board.png", &g_pBoard);
	LoadBitmapFromFile(L"snake.png", &g_pPlayerBitmap);
	LoadBitmapFromFile(L"apple.png", &g_pAppleBitmap);

	player = new Player(g_pPlayerBitmap);
	board = new Board(g_pBoard, g_pAppleBitmap, player, SCREEN_WIDTH, SCREEN_HEIGHT);
	board->Render(g_pRenderTarget);

	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			board->Input();
			board->Update();
			if (board->IsGameOver())
				break;
			Render();
		}
	}

	CleanupDevice();

	return (int)msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"D2DTutWindowClass";
	wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	g_hWnd = CreateWindow(L"D2DTutWindowClass", L"D2D1 Tutorial 1 : simple app",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!g_hWnd) return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//--------------------------------------------------------------------------------------
// Create Direct2D 
//--------------------------------------------------------------------------------------
HRESULT InitDevice(void)
{
	HRESULT hr = S_OK;

	RECT	rc;
	GetClientRect(g_hWnd, &rc);

	// Create a Direct2D factory 
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
	if (FAILED(hr)) return hr;

	// Craete a Direct2D render target
	hr = g_pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(g_hWnd,
			D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		&g_pRenderTarget);
	if (FAILED(hr)) return hr;

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pWICFactory));
	if (FAILED(hr)) return hr;


	return hr;
}

HRESULT LoadBitmapFromFile(PCWSTR _wcFileName, ID2D1Bitmap** _ppBitmap)
{
	HRESULT hr = S_OK;
	IWICBitmapDecoder* pDecoder = nullptr;

	hr = g_pWICFactory->CreateDecoderFromFilename(_wcFileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
	if (FAILED(hr)) return hr;

	IWICBitmapFrameDecode* pFrame = nullptr;
	hr = pDecoder->GetFrame(0, &pFrame);
	if (FAILED(hr)) return hr;

	IWICFormatConverter* pConverter = nullptr;
	hr = g_pWICFactory->CreateFormatConverter(&pConverter);
	if (FAILED(hr)) return hr;

	hr = pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
	if (FAILED(hr)) return hr;

	hr = g_pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, _ppBitmap);
	if (FAILED(hr)) return hr;

	if (pConverter) { pConverter->Release(); pConverter = nullptr; }
	if (pFrame) { pFrame->Release(); pFrame = nullptr; }
	if (pDecoder) { pDecoder->Release(); pDecoder = nullptr; }

	return hr;
}


//--------------------------------------------------------------------------------------
// Render the frame
//--------------------------------------------------------------------------------------
void Render()
{
	g_pRenderTarget->BeginDraw();
	g_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Aqua));

	//g_pRenderTarget->DrawBitmap(g_pBitmap);
	//sprite.Render(g_pRenderTarget, &g_pBoard);
	//board->Render(g_pRenderTarget);
	board->Render(g_pRenderTarget);

	g_pRenderTarget->EndDraw();
}

//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
	delete player;
	delete board;

	if (g_pAppleBitmap) g_pAppleBitmap->Release();
	if (g_pPlayerBitmap) g_pPlayerBitmap->Release();
	if (g_pBoard) g_pBoard->Release();
	if (g_pWICFactory) g_pWICFactory->Release();
	if (g_pRenderTarget) g_pRenderTarget->Release();
	if (g_pD2DFactory) g_pD2DFactory->Release();
	CoUninitialize();
}


