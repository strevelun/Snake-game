#include "CAnimationClip.h"
#include "Sprite.h"

CAnimationClip::CAnimationClip(ID2D1Bitmap* _bitmap, int _length, bool _isLoop)
{
	m_bitmap = _bitmap;
	m_length = _length;
	m_isLoop = _isLoop;
}

CAnimationClip::~CAnimationClip()
{

}

void CAnimationClip::AddSprite(D2D1_RECT_F _rect)
{
	m_vecSprite.push_back(new Sprite(m_bitmap, _rect));
}

void CAnimationClip::Play(ID2D1HwndRenderTarget* _target, D2D1_RECT_F _rect)
{
	m_vecSprite[m_curIdx++ % m_length]->Render(_target, _rect);
}