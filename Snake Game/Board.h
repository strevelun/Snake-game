#pragma once

#include "Sprite.h"
#include "Player.h"

// 2x2 ¹è¿­ Ä­¸¶´Ù dirÀÌ ÀúÀåµÊ.

class Board
{
	//Sprite* m_pSprite = nullptr;
	float m_boardWidth = 0, m_boardHeight = 0;
	ID2D1Bitmap** m_bitmap = nullptr;
	ID2D1Bitmap** m_appleBitmap = nullptr;
	D2D1_SIZE_F m_size;
	DIR* m_dirBoard;
	int m_board[16][16];
	Player* m_player;
	unsigned long m_timer = 0;
	unsigned long m_tick = 0;
	bool m_isGameOver = false;
	int m_appleXPos, m_appleYPos;

public:
	//Board();
	Board(ID2D1Bitmap** _bitmap, ID2D1Bitmap** _appleBitmap, Player* player, float _boardWidth, float _boardHeight);
	~Board();

	DIR GetMapDirAtPos(int _xpos, int _ypos)
	{
		return m_dirBoard[(_xpos * _ypos) + _xpos];
	}
	bool IsGameOver() const { return m_isGameOver; }
	void Input();
	void Update();
	void Render(ID2D1HwndRenderTarget* _target);
};

