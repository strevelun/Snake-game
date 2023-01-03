#pragma once
#include <vector>
#include <d2d1.h>

class Sprite;

class CAnimationClip
{
	std::vector<Sprite*> m_vecSprite;
	int m_length;
	bool m_isLoop;

	int m_curIdx = 0;

	ID2D1Bitmap* m_bitmap;

public:
	CAnimationClip(ID2D1Bitmap* _bitmap, int _length, bool _isLoop);
	~CAnimationClip();

	void AddSprite(D2D1_RECT_F _rect);
	void Play(ID2D1HwndRenderTarget* _target, D2D1_RECT_F _rect);
};