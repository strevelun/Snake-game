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
	for (int i = 0; i < bodyArr.size(); i++)
	{
		DIR temp = bodyArr[i]->dir;
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

void Player::AddBody()
{
	PosInfo* info = new PosInfo;

	// 꼬리가 향하는 방향의 반대방향 위치에 추가
	switch (bodyArr[m_bodyLength - 1]->dir)
	{
	case DIR_LEFT:
		info->xpos = bodyArr[m_bodyLength - 1]->xpos + 1;
		info->ypos = bodyArr[m_bodyLength - 1]->ypos;
		info->dir = DIR_LEFT;
		break;
	case DIR_UP:
		info->xpos = bodyArr[m_bodyLength - 1]->xpos;
		info->ypos = bodyArr[m_bodyLength - 1]->ypos + 1;
		info->dir = DIR_UP;
		break;
	case DIR_RIGHT:
		info->xpos = bodyArr[m_bodyLength - 1]->xpos - 1;
		info->ypos = bodyArr[m_bodyLength - 1]->ypos;
		info->dir = DIR_RIGHT;
		break;
	case DIR_DOWN:
		info->xpos = bodyArr[m_bodyLength - 1]->xpos;
		info->ypos = bodyArr[m_bodyLength - 1]->ypos - 1;
		info->dir = DIR_DOWN;
		break;
	}

	bodyArr.push_back(info);

	m_bodyLength++;
}