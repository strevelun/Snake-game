#pragma once

#include <unordered_map>
#include <string>
#include <d2d1.h> // not in cpp

class CAnimationClip;

class CAnimator
{
	std::unordered_map<std::string, CAnimationClip*> m_mapClip;

public:
	CAnimator();
	~CAnimator();
	
	void SetClip(std::string name, int length, bool isLoop);
	void AddClip(std::string name, CAnimationClip* clip);
	void ChangeClip();
	void Play(std::string name, ID2D1HwndRenderTarget* target, D2D1_RECT_F rect);
};