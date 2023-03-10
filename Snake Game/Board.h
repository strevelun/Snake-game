#pragma once

#include "Sprite.h"
#include "Player.h"

#include <unordered_map>
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")

class CBitmap;

class Board
{
	float m_boardWidth = 0, m_boardHeight = 0;
	//Sprite* m_backSprite = nullptr;
	//Sprite* m_appleSprite = nullptr;

	std::unordered_map<std::string, CBitmap*> m_mapBitmap;

	D2D1_SIZE_F m_size;
	DIR* m_dirBoard;
	int m_board[16][16];
	Player* m_player;
	unsigned long m_timer = 0;
	unsigned long m_tick = 0;
	bool m_isGameOver = false;
	int m_appleXPos, m_appleYPos;

	IWICImagingFactory* g_pWICFactory = nullptr;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;


public:
	Board(ID2D1HwndRenderTarget* _pRenderTarget, float _boardWidth, float _boardHeight);
	~Board();

	DIR GetMapDirAtPos(int _xpos, int _ypos)
	{
		return m_dirBoard[(_xpos * _ypos) + _xpos];
	}
	bool IsGameOver() const { return m_isGameOver; }
	void Input();
	void Update();
	void Render(ID2D1HwndRenderTarget* _target);
	HRESULT LoadBitmapFromFile(PCWSTR _wcFileName, CBitmap* _bitmap);
};

