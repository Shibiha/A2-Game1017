#pragma once
#ifndef __BACKGROUND_SCROLLING_H__
#define __BACKGROUND_SCROLLING_H__

#include "GameObject.h"
#include "Background.h"
#include <vector>

class BackgroundScrolling : public GameObject
{
public:
	BackgroundScrolling();
	~BackgroundScrolling();
	void Update();
	void Render();

	float playerSpeed = 0.0f;
private:
	std::vector<Background*> m_backgrounds;
};
#endif //!__BACKGROUND_SCROLLING_H__