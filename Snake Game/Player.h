#pragma once

#include <d2d1.h>
#include <vector>
#include "Sprite.h"

// 맨 앞 헤드가 방향을 표시해두면 그 뒤에 있던 몸통이 그 방향대로 따라가는 방식
// 방향키를 누르는 순간 map의 그 자리에 dir를 표시해둔다. 그리고 헤드는 
// 바로 방향을 바꿔서 아래로 내려감. 뒤따라오던 몸통은 오른쪽으로만 가다가 
// 즉 몸통들은 항상 map을 체크하고 있음.

class CBitmap;
class CAnimator;

enum DIR
{
	DIR_NONE,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN,
};

typedef struct _posInfo
{
	int xpos, ypos;
	DIR dir = DIR_RIGHT;
} PosInfo;

class Player
{
	DIR m_dir = DIR_RIGHT;
	int m_xpos = 0, m_ypos = 0;
	//Sprite* m_sprite;
	CBitmap* m_bitmap;
	D2D1_SIZE_F m_size;
	int m_bodyLength = 4;
	std::vector<PosInfo*> bodyArr;

	CAnimator* m_animator = nullptr;

public:
	Player(CBitmap* m_bitmap);
	~Player();

	int GetBodyLength() const { return m_bodyLength; }
	PosInfo* GetPosInfo(int idx) const;
	void Input();
	void Update();
	void Render(ID2D1HwndRenderTarget* _target);

	void AddBody();
};

