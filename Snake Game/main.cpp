
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include "Sprite.h"
#include "Board.h"
#include "CFrameTime.h"
#include "CBitmap.h"
#include <string>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#define SCREEN_WIDTH		800
#define SCREEN_HEIGHT		600

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = nullptr;
HWND                    g_hWnd = nullptr;

ID2D1Factory* g_pD2DFactory = nullptr;
ID2D1HwndRenderTarget* g_pRenderTarget = nullptr;

IDWriteFactory* g_pDWriteFactory;
IDWriteTextFormat* g_pDWTextFormat;

Board* board;
Player* player;

ID2D1Bitmap* g_pBoard = nullptr;
ID2D1Bitmap* g_pAppleBitmap = nullptr;

ID2D1SolidColorBrush* g_pBlackBrush;
//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT InitDevice(void);
void CleanupDevice(void);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void Render(void);


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


	board = new Board(g_pRenderTarget, SCREEN_WIDTH, SCREEN_HEIGHT);

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
			//CFrameTime::GetInstance()->Update();
			board->Input();


			if (CFrameTime::GetInstance()->Update())
			{
				float deltaTime = CFrameTime::GetInstance()->GetDeltaTime();

				board->Update();
				if (board->IsGameOver())
					break;
				Render();
			}
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
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
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

	//hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pWICFactory));
	//if (FAILED(hr)) return hr;
	
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&g_pDWriteFactory));
	if (FAILED(hr)) return hr;

	static const WCHAR fontName[] = L"Gabriola";
	const FLOAT fontSize = 50.0f;

	hr = g_pDWriteFactory->CreateTextFormat(fontName, NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		fontSize, L"en-us", &g_pDWTextFormat);
	if (FAILED(hr)) return hr;

	hr = g_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &g_pBlackBrush);

	return hr;
}


//--------------------------------------------------------------------------------------
// Render the frame
//--------------------------------------------------------------------------------------
void Render()
{
	g_pRenderTarget->BeginDraw();
	g_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Aqua));

	D2D1_SIZE_F rtSize = g_pRenderTarget->GetSize();

	board->Render(g_pRenderTarget);
	std::wstring str = std::to_wstring(CFrameTime::GetInstance()->GetFps());
	str.append(L" fps");
	const WCHAR* s = str.c_str();
	g_pRenderTarget->DrawTextW(s, str.length(), g_pDWTextFormat, D2D1::RectF(0, 0, rtSize.width, rtSize.height), g_pBlackBrush);

	g_pRenderTarget->EndDraw();
}

//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
	delete player;
	delete board;

	if (g_pRenderTarget) g_pRenderTarget->Release();
	if (g_pD2DFactory) g_pD2DFactory->Release();
	CoUninitialize();
}


