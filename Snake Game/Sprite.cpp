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

void Sprite::Add(float _x, float _y, float _w, float _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

void Sprite::Render(ID2D1HwndRenderTarget* _target, ID2D1Bitmap** _bitmap, float _tX, float _tY, float _tW, float _tH)
{
	_target->DrawBitmap(*_bitmap, D2D1::RectF(_tX, _tY, _tW, _tH), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(x, y, w, h));
}
