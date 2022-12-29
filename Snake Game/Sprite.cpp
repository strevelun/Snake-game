#include "Sprite.h"


Sprite::Sprite()
{
}

Sprite::Sprite(ID2D1Bitmap** _bitmap)
{
	//m_pBitmap = _bitmap;
}

Sprite::~Sprite()
{
}

void Sprite::Add(int _x, int _y, int _w, int _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

void Sprite::Render(ID2D1HwndRenderTarget* _target, ID2D1Bitmap** _bitmap)
{
	_target->DrawBitmap(*_bitmap, D2D1::RectF(100, 100, 200, 200), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(x, y, w, h));
}
