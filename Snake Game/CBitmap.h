#pragma once

#include <vector>
#include <d2d1.h>

#include "Sprite.h"

class CBitmap
{

	ID2D1Bitmap* m_pBitmap = nullptr;
	std::vector<Sprite*> m_vecSprite; // 스프라이트 수 
	D2D1_SIZE_F m_size = {};

public:
	CBitmap(PCWSTR name);
	ID2D1Bitmap* GetBitmap() const { return m_pBitmap; }
	void SetBitmap(ID2D1Bitmap* _bitmap);

	// 스프라이트 한 장만 출력
	void Render(ID2D1HwndRenderTarget* _target, float _tX, float _tY, float _tW, float _tH)
	{
		m_vecSprite[0]->Render(_target, _tX, _tY, _tW, _tH);
	}

	D2D1_SIZE_F GetSize() const { return m_size; }

	void AddSprite(float _x, float _y, float _w, float _h);
};

