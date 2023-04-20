#pragma once
#ifndef __PLAYERBULLET_H__
#define __PLAYERBULLET_H__
#include "GameObject.h"

class PlayerBullet : public GameObject
{
public:
	PlayerBullet(const SDL_FRect dst, float speed);
	void Update(const double dt);
	const float GetSpeed();
private:
	float m_speed;
};

#endif