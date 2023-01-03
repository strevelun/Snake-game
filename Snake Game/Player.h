#pragma once

#include <d2d1.h>
#include <vector>
#include "Sprite.h"

// �� �� ��尡 ������ ǥ���صθ� �� �ڿ� �ִ� ������ �� ������ ���󰡴� ���
// ����Ű�� ������ ���� map�� �� �ڸ��� dir�� ǥ���صд�. �׸��� ���� 
// �ٷ� ������ �ٲ㼭 �Ʒ��� ������. �ڵ������ ������ ���������θ� ���ٰ� 
// �� ������� �׻� map�� üũ�ϰ� ����.

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

