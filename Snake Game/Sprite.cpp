#include "Sprite.h"

Sprite::Sprite(ID2D1Bitmap* _bitmap, float _x, float _y, float _width, float _height)
{
	m_bitmap = _bitmap;
	m_rect = { _x,_y,_width, _height };
}

Sprite::Sprite(ID2D1Bitmap* _bitmap, D2D1_RECT_F _rect)
{
	m_bitmap = _bitmap;
	m_rect = _rect;
}

Sprite::~Sprite()
{
}

void Sprite::Render(ID2D1HwndRenderTarget* _target, float _tX, float _tY, float _tW, float _tH)
{
	_target->DrawBitmap(m_bitmap, D2D1::RectF(_tX, _tY, _tW, _tH), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, m_rect);
}
