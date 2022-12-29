#include "Board.h"
#include <cstdlib>
#include <ctime>

Board::Board(ID2D1Bitmap** _bitmap, ID2D1Bitmap** _appleBitmap, Player* _player, float _boardWidth, float _boardHeight)
{
	m_appleBitmap = _appleBitmap;
	m_bitmap = _bitmap;
	m_boardWidth = _boardWidth;
	m_boardHeight = _boardHeight;
	m_size = (*_bitmap)->GetSize();
	int boardSize = 192;
	m_dirBoard = new DIR[boardSize];
	
	for (int i = 0; i < boardSize; i++)
		m_dirBoard[i] = DIR_NONE;

	m_player = _player;
	m_tick = GetTickCount();

	m_appleXPos = 6;
	m_appleYPos = 6;
	m_board[m_appleYPos][m_appleXPos] = 1;

	srand(time(nullptr));
}

Board::~Board()
{
	delete m_player;
}

void Board::Input()
{
	PosInfo *info = m_player->GetPosInfo(0);
	int pos = (16 * info->ypos) + info->xpos;

	// 
	if (m_dirBoard[pos] == DIR_NONE)
	{
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
	}

}

void Board::Update()
{
	unsigned long tick = GetTickCount();
	m_timer += tick - m_tick;

	if (m_timer > 300)
	{
		m_timer = 0;

		int pos = 0;

		PosInfo* info = m_player->GetPosInfo(0);
		if (info->xpos < 0 || info->xpos >= 16 || info->ypos < 0 || info->ypos >= 12)
		{
			MessageBox(NULL, L"충돌!", L"충돌!", MB_OK);
			m_isGameOver = true;
		}

		// 몸통들은 m_board를 체크하고 마지막 몸통은 NONE으로 바꿈
		for (int i = 0; i < m_player->GetBodyLength(); i++)
		{
			info = m_player->GetPosInfo(i);

			pos = (16 * info->ypos) + info->xpos;

			if (m_dirBoard[pos] != DIR_NONE)
				info->dir = m_dirBoard[pos];
		}

		m_player->Update();

		m_dirBoard[pos] = DIR_NONE;

		// 아이템 습득
		info = m_player->GetPosInfo(0);
		if (m_board[info->ypos][info->xpos] == 1) // 그 자리에 아이템 존재하면
		{
			m_player->AddBody();
			m_board[info->ypos][info->xpos] = 0;
			m_appleXPos = rand() % 8;
			m_appleYPos = rand() % 12;
			m_board[m_appleYPos][m_appleXPos] = 1;
		}
	}
	
	m_tick = tick;

}

void Board::Render(ID2D1HwndRenderTarget* _target)
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			_target->DrawBitmap(*m_bitmap, D2D1::RectF((j * m_size.width / 2), i * m_size.height / 2, j * m_size.width / 2 + m_size.width / 2, i * m_size.height / 2 + m_size.height / 2), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(0, 0, m_size.width, m_size.height));
		}
	}

	_target->DrawBitmap(*m_appleBitmap, D2D1::RectF((m_appleXPos * m_size.width / 4), m_appleYPos * m_size.height / 2, m_appleXPos * m_size.width/4 + m_size.width / 4, m_appleYPos * m_size.height / 2 +  m_size.width / 4), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(0, 0, m_size.width, m_size.height));

	m_player->Render(_target);
}
