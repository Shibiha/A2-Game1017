#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player(const SDL_FRect dst, float speed);
	const float GetSpeed();
private:
	float m_speed;
};

#endif
