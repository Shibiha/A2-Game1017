#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "GameObject.h"

class Enemy : public Sprite
{
public:
	Enemy(const SDL_Rect src, const SDL_FRect dst, float speed);
	void Update(const double dt);
	const float GetSpeed();
private:
	float m_speed;
};

#endif