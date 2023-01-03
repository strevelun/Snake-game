#include "CBitmap.h"

CBitmap::CBitmap(PCWSTR name)
{

}

void CBitmap::AddSprite(float _x, float _y, float _w, float _h)
{
	m_vecSprite.push_back(new Sprite(m_pBitmap, _x, _y, _w, _h));
}

void CBitmap::SetBitmap(ID2D1Bitmap* _bitmap)
{
	m_pBitmap = _bitmap;
	m_size = _bitmap->GetSize();
}