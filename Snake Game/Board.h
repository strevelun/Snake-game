#pragma once

#include "Sprite.h"
#include "Player.h"

// 2x2 ¹è¿­ Ä­¸¶´Ù dirÀÌ ÀúÀåµÊ.

class Board
{
	//Sprite* m_pSprite = nullptr;
	float m_boardWidth = 0, m_boardHeight = 0;
	ID2D1Bitmap** m_bitmap = nullptr;
	D2D1_SIZE_F m_size;
	DIR* m_board;
	Player* m_player;
	unsigned long m_timer = 0;
	unsigned long m_tick = 0;

public:
	//Board();
	Board(ID2D1Bitmap** _bitmap, Player* player, float _boardWidth, float _boardHeight);
	~Board();

	DIR GetMapDirAtPos(int _xpos, int _ypos)
	{
		return m_board[(_xpos * _ypos) + _xpos];
	}
	void Input();
	void Update();
	void Render(ID2D1HwndRenderTarget* _target);
};

