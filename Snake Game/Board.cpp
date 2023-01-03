#include <cstdlib>
#include <ctime>
#include <string>

#include "Board.h"
#include "CFrameTime.h"
#include "CBitmap.h"


Board::Board(ID2D1HwndRenderTarget* _pRenderTarget, float _boardWidth, float _boardHeight)
{
	//m_backSprite = new Sprite(_bitmap);
	//m_appleSprite = new Sprite(_appleBitmap);
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pWICFactory));


	m_pRenderTarget = _pRenderTarget;

	CBitmap *board = new CBitmap(L"board.png");
	CBitmap *snake = new CBitmap(L"snake.png");
	CBitmap *apple = new CBitmap(L"apple.png");

	LoadBitmapFromFile(L"board.png", board);
	LoadBitmapFromFile(L"snake.png", snake);
	LoadBitmapFromFile(L"apple.png", apple);

	board->AddSprite(0, 0, 200, 100);
	snake->AddSprite(0, 0, 100, 100);
	apple->AddSprite(0, 0, 100, 100);

	m_mapBitmap.insert(std::make_pair("board", board));
	m_mapBitmap.insert(std::make_pair("snake", snake));
	m_mapBitmap.insert(std::make_pair("apple", apple));


	m_player = new Player(m_mapBitmap["snake"]);

	m_boardWidth = _boardWidth;
	m_boardHeight = _boardHeight;
	m_size = m_mapBitmap["board"]->GetSize();
	int boardSize = 192;
	m_dirBoard = new DIR[boardSize];
	
	for (int i = 0; i < boardSize; i++)
		m_dirBoard[i] = DIR_NONE;

	m_tick = GetTickCount();

	m_appleXPos = 6;
	m_appleYPos = 6;
	m_board[m_appleYPos][m_appleXPos] = 1;

	srand(time(nullptr));

	CFrameTime::GetInstance()->SetFrameLimit(30);

}

Board::~Board()
{
	delete m_player;

	// m_mapBitmap 해제

}

void Board::Input()
{
	PosInfo *info = m_player->GetPosInfo(0);
	int pos = (16 * info->ypos) + info->xpos;

	//if (m_dirBoard[pos] == DIR_NONE)

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		if(info->dir != DIR_DOWN)
			m_dirBoard[pos] = DIR_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		if (info->dir != DIR_UP)
			m_dirBoard[pos] = DIR_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		if (info->dir != DIR_RIGHT)
			m_dirBoard[pos] = DIR_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		if (info->dir != DIR_LEFT)
			m_dirBoard[pos] = DIR_RIGHT;

	info->dir = DIR_NONE;
}

void Board::Update()
{
	//unsigned long tick = GetTickCount();
	//m_timer += tick - m_tick;

	//if (m_timer > 400)
	{
	//	m_timer = 0;

		int pos = 0;

		PosInfo* headInfo = m_player->GetPosInfo(0);
		if (headInfo->xpos < 0 || headInfo->xpos >= 16 || headInfo->ypos < 0 || headInfo->ypos >= 12)
		{
			MessageBox(NULL, L"충돌!", L"충돌!", MB_OK);
			m_isGameOver = true;
			return;
		}

		PosInfo* info;

		for (int i = 0; i < m_player->GetBodyLength(); i++)
		{
			info = m_player->GetPosInfo(i);

			pos = (16 * info->ypos) + info->xpos;

			// 몸통들은 m_board를 체크하고 마지막 몸통은 NONE으로 바꿈
			if (m_dirBoard[pos] != DIR_NONE)
				info->dir = m_dirBoard[pos];
		}

		m_player->Update();

		m_dirBoard[pos] = DIR_NONE;

		for (int i = 1; i < m_player->GetBodyLength(); i++)
		{
			info = m_player->GetPosInfo(i);

			// 머리가 몸통이랑 충돌 여부 체크
			if (headInfo->xpos == info->xpos && headInfo->ypos == info->ypos)
			{
				MessageBox(NULL, L"바디와 충돌!", L"바디와 충돌!", MB_OK);
				m_isGameOver = true;
				return;
			}
		}

		// 아이템 습득
		if (m_board[headInfo->ypos][headInfo->xpos] == 1) // 그 자리에 아이템 존재하면
		{
			m_player->AddBody();
			m_board[headInfo->ypos][headInfo->xpos] = 0;
			m_appleXPos = rand() % 8;
			m_appleYPos = rand() % 12;
			m_board[m_appleYPos][m_appleXPos] = 1;
		}
	}
	//m_tick = tick;
}

void Board::Render(ID2D1HwndRenderTarget* _target)
{
	for (int i = 0; i < 12; i++)
		for (int j = 0; j < 8; j++)
			m_mapBitmap["board"]->Render(_target, (j * m_size.width / 2), i * m_size.height / 2, j * m_size.width / 2 + m_size.width / 2, i * m_size.height / 2 + m_size.height / 2);
			
	// 사과출력
	m_mapBitmap["apple"]->Render(_target, (m_appleXPos * m_size.width / 4), m_appleYPos * m_size.height / 2, m_appleXPos * m_size.width / 4 + m_size.width / 4, m_appleYPos * m_size.height / 2 + m_size.width / 4);
	
	m_player->Render(_target);
}

HRESULT Board::LoadBitmapFromFile(PCWSTR _wcFileName, CBitmap* _bitmap)
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

	ID2D1Bitmap* bitmap = nullptr;

	hr = m_pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, &bitmap);
	if (FAILED(hr)) return hr;

	_bitmap->SetBitmap(bitmap);


	if (pConverter) { pConverter->Release(); pConverter = nullptr; }
	if (pFrame) { pFrame->Release(); pFrame = nullptr; }
	if (pDecoder) { pDecoder->Release(); pDecoder = nullptr; }

	return hr;

}
