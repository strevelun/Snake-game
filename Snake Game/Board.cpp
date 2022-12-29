#include "Board.h"


Board::Board(ID2D1Bitmap** _bitmap, Player* _player, float _boardWidth, float _boardHeight)
{
	m_bitmap = _bitmap;
	m_boardWidth = _boardWidth;
	m_boardHeight = _boardHeight;
	m_size = (*_bitmap)->GetSize();
	m_board = new DIR[(m_boardHeight / m_size.height * 2) * (m_boardWidth / m_size.width * 2)];

	m_player = _player;
	m_tick = GetTickCount();
}

Board::~Board()
{
	delete m_player;
}

void Board::Input()
{
	PosInfo *info = m_player->GetPosInfo(0);

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_board[(info->xpos * info->ypos) + info->xpos] = DIR_UP;
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_board[(info->xpos * info->ypos) + info->xpos] = DIR_DOWN;
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_board[(info->xpos * info->ypos) + info->xpos] = DIR_LEFT;
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_board[(info->xpos * info->ypos) + info->xpos] = DIR_RIGHT;

}

void Board::Update()
{
	unsigned long tick = GetTickCount();
	m_timer += tick - m_tick;

	if (m_timer > 500)
	{
		m_player->Update();
		m_timer = 0;
	}
	
	m_tick = tick;

	// 좌표 갱신

	// 몸통들은 m_board를 체크하고 마지막 몸통은 NONE으로 바꿈
	for (int i = 0; i < m_player->GetBodyLength(); i++)
	{
		PosInfo* info = m_player->GetPosInfo(i);
		if (m_board[(info->xpos * info->ypos) + info->xpos] != DIR_NONE)
		{
			info->dir = m_board[(info->xpos * info->ypos) + info->xpos];
		}
	}
}

void Board::Render(ID2D1HwndRenderTarget* _target)
{
	for (int i = 0; i < m_boardHeight / m_size.height*2; i++)
	{
		for (int j = 0; j < m_boardWidth / m_size.width*2; j++)
		{
			_target->DrawBitmap(*m_bitmap, D2D1::RectF((j * m_size.width/2), i * m_size.height / 2, j *m_size.width/2 + m_size.width/2, i * m_size.height / 2 + m_size.height/2), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(0, 0, m_size.width, m_size.height));
		}
	}

	m_player->Render(_target);
}
