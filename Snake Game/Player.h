#pragma once

#include <d2d1.h>
#include <vector>

// �� �� ��尡 ������ ǥ���صθ� �� �ڿ� �ִ� ������ �� ������ ���󰡴� ���
// ����Ű�� ������ ���� map�� �� �ڸ��� dir�� ǥ���صд�. �׸��� ���� 
// �ٷ� ������ �ٲ㼭 �Ʒ��� ������. �ڵ������ ������ ���������θ� ���ٰ� 
// �� ������� �׻� map�� üũ�ϰ� ����.

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
	ID2D1Bitmap** m_bitmap = nullptr;
	D2D1_SIZE_F m_size;
	int m_bodyLength = 4;
	std::vector<PosInfo*> bodyArr;

public:
	Player(ID2D1Bitmap** m_bitmap);
	~Player();

	int GetBodyLength() const { return m_bodyLength; }
	PosInfo* GetPosInfo(int idx) const;
	void Input();
	void Update();
	void Render(ID2D1HwndRenderTarget* _target);

	void AddBody();
};

