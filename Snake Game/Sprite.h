#pragma once

#include <d2d1.h>


class Sprite
{
	int x, y, w, h;
	D2D1_RECT_F m_rect;
	ID2D1Bitmap* m_bitmap;

public:
	Sprite();
	Sprite(ID2D1Bitmap** _bitmap);
	Sprite(ID2D1Bitmap* _bitmap, D2D1_RECT_F& _rect)
	{
		m_rect = _rect;
		m_bitmap = _bitmap;
	}
	~Sprite();

	void Add(int _x, int _y, int _w, int _h);
	void Render(ID2D1HwndRenderTarget* _target, ID2D1Bitmap** _bitmap);
	void Render(ID2D1HwndRenderTarget* _rt, const D2D1_RECT_F& _destRect)
	{
		_rt->DrawBitmap(m_bitmap, _destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, m_rect);
	}
};

