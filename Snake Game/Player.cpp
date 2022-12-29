#include "Player.h"

Player::Player(ID2D1Bitmap** _bitmap)
{
	m_bitmap = _bitmap;
	m_size = (*_bitmap)->GetSize();
	m_size.width /= 2;
	m_size.height /= 2;

	// 머리 시작 위치 좌표
	m_xpos = 5;
	m_ypos = 2;

	PosInfo *info;

	for (int i = 0; i < 4; i++)
	{
		info = new PosInfo;
		info->xpos = m_xpos - i;
		info->ypos = 2;
		bodyArr.push_back(info);
	}
}

Player::~Player()
{
}

PosInfo *Player::GetPosInfo(int idx) const
{ 
	if (idx < 0 || idx >= bodyArr.size())
		return nullptr;

	return bodyArr[idx];
}

void Player::Input()
{

}

void Player::Update()
{
	// 각 바디는 map 2차원 배열을 체크한다음 map에 현재위치에 있는 방향을 업데이트

	if (m_dir != bodyArr[0]->dir)
	{
		
	}

	for (int i = 0; i < bodyArr.size(); i++)
	{
		switch (bodyArr[i]->dir)
		{
		case DIR_LEFT:
			bodyArr[i]->xpos -= 1;
			break;
		case DIR_UP:
			bodyArr[i]->ypos -= 1;
			break;
		case DIR_RIGHT:
			bodyArr[i]->xpos += 1;
			break;
		case DIR_DOWN:
			bodyArr[i]->ypos += 1;
			break;
		case DIR_NONE:

			break;
		}
	}
}

void Player::Render(ID2D1HwndRenderTarget* _target)
{
	float xpos;
	float ypos;

	for (int i = 0; i < bodyArr.size(); i++)
	{
		PosInfo* info = bodyArr[i];
		xpos = info->xpos * m_size.width;
		ypos = info->ypos * m_size.height;
		_target->DrawBitmap(*m_bitmap, D2D1::RectF(xpos, ypos, xpos + m_size.width, ypos + m_size.height), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(0, 0, m_size.width, m_size.height));
	}
}
