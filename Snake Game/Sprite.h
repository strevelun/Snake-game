#pragma once

#include <d2d1.h>

class Sprite
{
	float x = 0, y = 0, w = 0, h = 0;
	D2D1_RECT_F m_rect = {};
	D2D1_SIZE_F m_size = {};
	ID2D1Bitmap* m_bitmap = nullptr;

public:
	Sprite(ID2D1Bitmap* _bitmap);
	Sprite(ID2D1Bitmap* _bitmap, D2D1_RECT_F& _rect)
	{
		m_rect = _rect;
		m_bitmap = _bitmap;
	}
	~Sprite();

	void Add(float _x, float _y, float _w, float _h);
	// 스프라이트가 가지고 있는 비트맵에서 잘라내고 뿌리고 싶은 위치와 크기 지정
	void Render(ID2D1HwndRenderTarget* _target, float _tX, float _tY, float _tW, float _tH);
	void Render(ID2D1HwndRenderTarget* _rt, const D2D1_RECT_F& _destRect)
	{
		_rt->DrawBitmap(m_bitmap, _destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, m_rect);
	}

	D2D1_SIZE_F GetSize() const { return m_size; }
};

