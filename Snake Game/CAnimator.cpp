#include "CAnimator.h"
#include "CAnimationClip.h"

#include <Windows.h>

CAnimator::CAnimator()
{

}

CAnimator::~CAnimator()
{

}

void CAnimator::SetClip(std::string name, int length, bool isLoop)
{

}

void CAnimator::AddClip(std::string name, CAnimationClip* clip)
{	
	m_mapClip.insert(make_pair(name, clip));
}


void CAnimator::Play(std::string name, ID2D1HwndRenderTarget* target, D2D1_RECT_F rect)
{
	if (m_mapClip.find(name) == m_mapClip.end())
		return;

	m_mapClip[name]->Play(target, rect);
}

